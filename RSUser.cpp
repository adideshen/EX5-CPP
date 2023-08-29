
#include "RSUser.h"
#include "RecommenderSystem.h"

RSUser::RSUser (const std::string& username, const rank_map& rank_map,
               const std::shared_ptr<RecommenderSystem>& recommender_system)
{
  _username = username;
  _rank_map = rank_map;
  _sp_recommender_system = recommender_system;
}

void RSUser::add_movie_to_rs(const std::string& name, int year,
                             const std::vector<double> &features,
                             double rate)
{
  sp_movie movie_to_add = _sp_recommender_system->get_movie (name, year);
  if (movie_to_add == nullptr)
  {
    movie_to_add = _sp_recommender_system->add_movie (name, year, features);
  }
  auto res = _rank_map.find (movie_to_add);
  if (res == _rank_map.end())
  {
    _rank_map.insert (std::pair<sp_movie, double> (movie_to_add, 0));
  }
  _rank_map.find (movie_to_add)->second = rate;
}

sp_movie RSUser::get_recommendation_by_content() const
{
  return _sp_recommender_system->recommend_by_content(*this);
}

sp_movie RSUser::get_recommendation_by_cf(int k) const
{
  return _sp_recommender_system->recommend_by_cf (*this, k);
}

double RSUser::get_prediction_score_for_movie(const std::string& name, int
year, int k) const
{
  sp_movie movie_to_predict = _sp_recommender_system->get_movie (name, year);
  return _sp_recommender_system->predict_movie_score (*this,
                                                      movie_to_predict, k);
}

std::ostream& operator<< (std::ostream& os, const RSUser& rs_user)
{
  os << "name: " << rs_user.get_name() << std::endl;
  os << *(rs_user._sp_recommender_system) << std::endl;
  return os;
}
