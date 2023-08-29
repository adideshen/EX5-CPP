
#ifndef SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
#define SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
#include "RSUser.h"

class RecommenderSystem;

typedef std::map<sp_movie, std::vector<double>, equal_func> rs_map;
typedef std::multimap<double, sp_movie> ranked_sp_movie;

class RecommenderSystem
{
public:

  RecommenderSystem() : _recommender_system(sp_movie_comp) {}

  /**
   * adds a new movie to the system
   * @param name name of movie
   * @param year year it was made
   * @param features features for movie
   * @return shared pointer for movie in system
   */
  sp_movie add_movie(const std::string& name, int year, const
  std::vector<double>& features);

  /**
   * gets a shared pointer to movie in system
   * @param name name of movie
   * @param year year movie was made
   * @return shared pointer to movie in system
   */
  sp_movie get_movie(const std::string &name, int year) const;

  /**
   * a function that calculates the movie with highest score based on movie
   * features
   * @param ranks user ranking to use for algorithm
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_content(const RSUser& user);

  /**
   * a function that calculates the movie with highest predicted score based on
   * ranking of other movies
   * @param ranks user ranking to use for algorithm
   * @param k
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_cf(const RSUser& user, int k);

  /**
   * Predict a user rating for a movie given argument using item cf procedure
   * with k most similar movies.
   * @param user_rankings: ranking to use
   * @param movie: movie to predict
   * @param k:
   * @return score based on algorithm as described in pdf
   */
  double predict_movie_score(const RSUser &user, const sp_movie &movie,
                                                int k);
  /**
    * output stream operator
   * @param os the output stream
   * @param user the recommender system
   * @return output stream
   */
  friend std::ostream& operator<< (std::ostream& s, const RecommenderSystem&
  rs);

 private:
  rs_map _recommender_system;

  /**
   * This function returns the average of all the values in a map.
   * @param rank_map
   * @return the average
   */
  static double ranks_average (const rank_map& rank_map);

  /**
   * This function returns the vector's norm.
   * @param vec
   * @return the norm.
   */
  static double vector_norm (const std::vector<double>& vec);

  /**
   * This function returns the inner product space of two vectors
   * @param vec1
   * @param vec2
   * @return the inner product space
   */
  static double vectors_multiply (const std::vector<double>& vec1, const
  std::vector<double>& vec2);

  /**
   * This function finds the recommended movie given the user's preferences
   * vector.
   * @param vec user's preferences vector.
   * @return sp_movie
   */
  sp_movie find_recommend_by_content (const std::vector<double>& vec,
                                      const RSUser& user) const;

  /**
   * This function finds the k most similar movies to a given movie.
   * @param user
   * @param movie
   * @param k
   * @return multimap that  it's keys are doubles that indicating the
   * similarity and it's values are smart pointer to the similar movie.
   */
  ranked_sp_movie movie_similarity(const RSUser& user, const sp_movie&
  movie, int k);
  };


#endif //SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
