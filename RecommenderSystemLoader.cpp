
#include <iostream>
#include <fstream>
#include <sstream>
#include "RecommenderSystemLoader.h"


#define INVALID_RANK "Movie's rank must be between 0 and 10"
#define ERROR_INVALID_INPUT "Error: Failed to retrieve input."
#define TEN 10


/**
 * This function receives the data of a line.
 * @param line a string of a line.
 * @param rs_sp vector of pointers to user-rated movies.
 */
void split_line (std::string& line, std::unique_ptr<RecommenderSystem>& rs_sp)
{
  for (char & i : line)
  {
    if (i == '-')
    {
      i = ' ';
    }
  }
  std::vector<double> features;
  std::stringstream stream_line(line);
  std::string movie;
  int year;
  double rank;
  while (stream_line >> movie >> year)
  {
    while (stream_line >> rank)
    {
      if ((rank < 1) || (rank > TEN))
      {
        throw std::invalid_argument (INVALID_RANK);
      }
      features.push_back (rank);
    }
  }
  rs_sp->add_movie (movie, year, features);
}

std::unique_ptr<RecommenderSystem>
RecommenderSystemLoader::create_rs_from_movies_file (const
std::string &movies_file_path) noexcept (false)
{
  std::unique_ptr<RecommenderSystem> rs_sp =
      std::make_unique<RecommenderSystem>(RecommenderSystem());
  std::ifstream input(movies_file_path);
  std::stringstream input_txt;
  if (!input)
  {
    throw std::runtime_error (ERROR_INVALID_INPUT);
  }
  input_txt << input.rdbuf();
  input.close();
  std::string line;
  while (std::getline (input_txt, line))
  {
    split_line (line, rs_sp);
  }
  return rs_sp;
}

