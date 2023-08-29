
#include <algorithm>
#include <cmath>
#include "RecommenderSystem.h"


sp_movie RecommenderSystem::add_movie(const std::string& name,int year ,const
std::vector<double>& features)
{
  sp_movie new_sp_movie = std::make_shared<Movie>(Movie(name, year));
  _recommender_system.insert(std::pair<sp_movie, std::vector<double>>
      (new_sp_movie, features));
  return new_sp_movie;
}

sp_movie RecommenderSystem::get_movie(const std::string &name, int year) const
{
  sp_movie new_sp_movie = std::make_shared<Movie>(Movie(name, year));
  auto res = _recommender_system.find(new_sp_movie);
  if (res == _recommender_system.end())
  {
    return nullptr;
  }
  return res->first;
}

double RecommenderSystem::ranks_average(const rank_map& user_ranks)
{
  double sum = 0;
  double num_of_elem = 0;
  for (const auto& pair : user_ranks)
  {
    if (pair.second != 0)
    {
      sum += pair.second;
      num_of_elem++;
    }
  }
  return sum / num_of_elem;
}

double RecommenderSystem::vector_norm(const std::vector<double>& vec)
{
  double norm = 0;
  for (double d: vec)
  {
    norm += d * d;
  }
  return sqrt (norm);
}

double RecommenderSystem::vectors_multiply (const std::vector<double>& vec1,
                                            const std::vector<double>& vec2)
{
  double multiply = 0;
  for (size_t i = 0; i < vec1.size(); i++)
  {
    multiply += vec1[i] * vec2[i];
  }
  return multiply;
}

sp_movie RecommenderSystem::find_recommend_by_content
(const std::vector<double>& vec, const RSUser& user) const
{
  sp_movie recommend_movie;
  bool first_movie = true;
  double best_match = 0;
  for (const auto& pair : _recommender_system)
  {
    auto find_movie = user.get_ranks().find (pair.first);
    if ((find_movie == user.get_ranks().end()) || (find_movie->second == 0))
    {
      double vectors_molt = vectors_multiply (vec, pair.second);
      double norm_molt = vector_norm(vec) * vector_norm(pair.second);
      if (((vectors_molt / norm_molt) > best_match) || (first_movie))
      {
        best_match = vectors_molt / norm_molt;
        recommend_movie = pair.first;
        first_movie = false;
      }
    }
  }
  return recommend_movie;
}

sp_movie RecommenderSystem::recommend_by_content(const RSUser& user)
{
  double average_rank = ranks_average (user.get_ranks());
  rank_map new_rank_map = user.get_ranks();
  unsigned long num_of_features = 0;
  for (auto& pair : new_rank_map)
  {
    if (pair.second != 0)
    {
      pair.second -= average_rank;
      if (num_of_features == 0)
      {
        num_of_features = (_recommender_system[pair.first]).size();
      }
    }
  }
  std::vector<double> recommendation_vec(num_of_features);
  for (auto& pair : new_rank_map)
  {
    if (pair.second != 0)
    {
      double k = pair.second;
      std::vector<double> cur_movie =
          _recommender_system.find(pair.first)->second;
      std::for_each (cur_movie.begin(), cur_movie.end(),
                     [k](double &c){c *= k;});
      for (unsigned long i = 0; i < num_of_features; i++)
      {
        recommendation_vec[i] += cur_movie[i];
      }
    }
  }
  return find_recommend_by_content (recommendation_vec, user);
}

ranked_sp_movie RecommenderSystem::movie_similarity(const RSUser& user, const
sp_movie& movie, int k)
{
  ranked_sp_movie similarity_rank;
  ranked_sp_movie k_similar;
  for (const auto &pair: user.get_ranks ())
  {
    if (pair.second != 0)
    {
      double multi_vec = (vectors_multiply (
          _recommender_system.find (movie)->second,
          _recommender_system.find (pair.first)->second));
      double multi_norm = vector_norm (
          _recommender_system.find (movie)->second) *
                          vector_norm (_recommender_system.find
                          (pair.first)->second);
      similarity_rank.insert (std::pair<double, sp_movie> ((multi_vec /
                                                            multi_norm),
                                                           pair.first));
    }
  }
  unsigned long counter = 0;
  for (auto &pair: similarity_rank)
  {
    if (counter >= similarity_rank.size () - k)
    {
      k_similar.insert (pair);
    }
    counter++;
  }
  return k_similar;
}

double RecommenderSystem::predict_movie_score(const RSUser &user, const
sp_movie &movie, int k)
{
  ranked_sp_movie similarity_rank = movie_similarity(user, movie, k);
  double numerator = 0;
  double denominator = 0;
  for (auto & it1 : similarity_rank)
  {
    for (const auto & it2 : user.get_ranks())
    {
      if (it1.second == it2.first)
      {
        numerator += it1.first * it2.second;
        denominator += it1.first;
      }
    }
  }
  return numerator/denominator;
}

sp_movie RecommenderSystem::recommend_by_cf(const RSUser& user, int k)
{
  ranked_sp_movie ranking_prediction;
  for (const auto& pair : _recommender_system)
  {
    auto find_movie = user.get_ranks().find (pair.first);
    if ((find_movie == user.get_ranks().end()) || (find_movie->second == 0))
    {
      double movie_rank = predict_movie_score(user, pair.first, k);
      ranking_prediction.insert(std::pair<double, sp_movie> (movie_rank,
                                                             pair.first));
    }
  }
  sp_movie recommend_movie;
  for (auto & it : ranking_prediction)
  {
    recommend_movie = it.second;
  }
  return recommend_movie;
}

std::ostream& operator<< (std::ostream& os, const RecommenderSystem& rs)
{
  for (const auto& pair : rs._recommender_system)
  {
    os << *(pair.first);
  }
  return os;
}