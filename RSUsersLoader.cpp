
#include <iostream>
#include <fstream>
#include <sstream>
#include "RSUsersLoader.h"
#include "RecommenderSystem.h"

#define INVALID_RANK "Movie's rank must be between 0 and 10"
#define ERROR_INVALID_INPUT "Error: Failed to retrieve input."
#define TEN 10

/**
 * This function receives the data of the first line.
 * @param line a string of a line.
 * @param movies_vec vector of pointers to user-rated movies.
 */
void split_first_line (std::string& line,
                                 std::vector<sp_movie>& movies_vec)
{
  for (char & i : line)
  {
    if (i == '-')
    {
      i = ' ';
    }
  }
  std::stringstream stream_line(line);
  std::string movie;
  int year;
  while (stream_line >> movie >> year)
  {
    sp_movie new_movie = std::make_shared<Movie> (Movie (movie, year));
    movies_vec.push_back (new_movie);
  }
}

/**
 * This function receives the data of a line, excluded the first line.
 * @param line a string of a line.
 * @param rs smart pointer to recommender system.
 * @param movies_vec vector of pointers to user-rated movies.
 * @param users_vec vector of pointers to users.
 */
void split_lines (std::string& line,
                            const std::shared_ptr<RecommenderSystem>& rs,
                            std::vector<sp_movie>& movies_vec,
                            std::vector<RSUser>& users_vec)
{
  rank_map r_m (0, sp_movie_hash, sp_movie_equal);
  std::stringstream stream_line (line);
  std::string name;
  stream_line >> name;
  std::string string_rank;
  double rank;
  int counter = 0;
  while (stream_line >> string_rank)
  {
    if (string_rank == "NA")
    {
      rank = 0;
    }
    else
    {
      rank = std::stod (string_rank);
      if ((rank < 1) || (rank > TEN))
      {
        throw std::invalid_argument (INVALID_RANK);
      }
    }
    r_m[movies_vec[counter]] = rank;
    counter++;
  }
  users_vec.emplace_back (RSUser (name, r_m, rs));
}

std::vector<RSUser>
RSUsersLoader::create_users_from_file (const std::string &users_file_path,
                                       std::unique_ptr<RecommenderSystem> rs)
                                       noexcept (false)
{
  std::shared_ptr<RecommenderSystem> new_rs =
      std::make_shared<RecommenderSystem>(*rs);
  std::vector<RSUser> users_vec;
  std::vector<sp_movie> movies_vec;
  std::ifstream input (users_file_path);
  std::stringstream input_txt;
  if (!input)
  {
    throw std::runtime_error (ERROR_INVALID_INPUT);
  }
  input_txt << input.rdbuf();
  input.close();
  std::string line;
  bool flag_first_line = true;
  while (std::getline (input_txt, line))
  {
    if (flag_first_line)
    {
      split_first_line (line, movies_vec);
      flag_first_line = false;
    }
    else
    {
      split_lines (line, new_rs, movies_vec, users_vec);
    }
  }
  return users_vec;
}