#ifndef GAME_HPP
#define GAME_HPP

#include <array>
#include <vector>
#include <map>
#include <string>

#include "engine/model.hpp"
#include "engine/mesh.hpp"
#include "engine/texture.hpp"

#include "chess/chess.hpp"
#include "utilities.hpp"


class Game {
public:
  Game();

  std::vector<Piece*> get_pieces();

  void Init();
  void Display(GLuint program_id);

  void UpdatePieceWorldPosition(int id, float x, float z);

  void DisactivatePiece(Piece &piece);

public:
  chschr::Chess* chess;

  std::vector<int> active_fields;

private:
  glm::vec3 IndexToPosition(int id);

  void LoadModelsOBJ(std::vector<std::string> names);
  void LoadTexturesJPG(std::vector<std::string> names);

private:
  std::map<std::string, Model*> models_;
  std::map<std::string, Texture*> textures_;

  std::vector<Piece*> pieces_;
  std::array<Shape*, 64> squares_;

  float off_rank_white = -8.0;
  float off_rank_black = -8.0;
};

#endif
