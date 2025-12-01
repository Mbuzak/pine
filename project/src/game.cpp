#include "game.hpp"


Game::Game() {
  chess = new chschr::Chess();
}

std::vector<Piece*> Game::get_pieces() {
  return pieces_;
}

void Game::Init() {
  std::vector<std::string> model_files = {
    "square", "pawn", "knight", "bishop", "rook", "king", "queen", "chessboard"
  };
  
  LoadModelsOBJ(model_files);

  std::vector<std::string> texture_files = {"white", "black", "chessboard"};

  LoadTexturesJPG(texture_files);

  for (int i = 0; i < squares_.size(); i++) {
    squares_[i] = new Shape(models_.at("square"), IndexToPosition(i));

    if (((i % 8) + (i / 8)) % 2 == 0) {
      squares_[i]->material_.ambient.r = 0.5;
      squares_[i]->material_.ambient.g = 0.5;
      squares_[i]->material_.ambient.b = 0.5;
    }
  }

  std::string name;
  std::string colour;
  float x, y = 0.1, z;

  for (int i = 0; i < chess->mBoard.size(); i++) {
    name = chess->pieceName.at(chess->mBoard[i]);
    if (chess->isWhite(i / 8, i % 8)) colour = "white";
    else colour = "black";

    if (name == "x")
        continue;

    Piece *piece = new Piece(i, models_.at(name), textures_.at(colour));

    if (colour == "white") {
      piece->angle_.y = 3.2;
      piece->colour = "white";
    }
    else {
      piece->colour = "black";
    }

    pieces_.push_back(piece);
  }
}

void Game::LoadModelsOBJ(std::vector<std::string> names) {
  for (std::string &name: names) {
    models_.insert(std::pair<std::string, Model*>(name, new Model(name)));
  }
}

void Game::LoadTexturesJPG(std::vector<std::string> names) {
  for (std::string &name: names) {
    std::string file = name + ".jpg";
    textures_.insert(std::pair<std::string, Texture*>(name, new Texture(file.c_str())));
  }
}

void Game::Display(GLuint program_id) {
  for (int &value: active_fields) {
    //std::cout << value << "\n";
    glUniform1i(glGetUniformLocation(program_id, "active_field"), true);
    squares_[value]->Display(program_id);
    glUniform1i(glGetUniformLocation(program_id, "active_field"), false);
  }

  for (int i = 0; i < squares_.size(); i++) {
    squares_[i]->Display(program_id);
  }

  for (int i = 0; i < pieces_.size(); ++i) {
    glStencilFunc(GL_ALWAYS, i + 1, 0xFF);

    pieces_[i]->Display(program_id);
  }

  if (selected_id >= 0) {
    pieces_[selected_id]->DisplayOutline(program_id);
  }
}

void Game::UpdatePieceWorldPosition(int id, float x, float z) {
  get_pieces()[id]->position_.x = x;
  get_pieces()[id]->position_.z = z;
}

void Game::DisactivatePiece(Piece &piece) {
  if (piece.colour == "white") {
    if (off_rank_white < 8.0) {
      piece.position_.x = 12.0;
      piece.position_.z = off_rank_white;
    }
    else {
      piece.position_.x = 14.0;
      piece.position_.z = off_rank_white - 16.0;
    }
    
    off_rank_white += 2.0;
  }
  else {
    if (off_rank_black < 8.0) {
      piece.position_.x = -12.0;
      piece.position_.z = off_rank_black;
    }
    else {
      piece.position_.x = -14.0;
      piece.position_.z = off_rank_black - 16.0;
    }
    off_rank_black += 2.0;
  }

  piece.is_active = false;
}

glm::vec3 Game::IndexToPosition(int id) {
  glm::vec3 position;

  position.x = ((id % 8) - 4) * 2.25 + 1.12;
  position.y = 0.15;
  position.z = ((id / 8) - 4) * 2.25 + 1.12;

  return position;
}
