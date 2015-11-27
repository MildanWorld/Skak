#ifdef SFML_STATIC
#pragma comment(lib, "glew.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")  
#endif // SFML_STATIC

/* Inspirationer
ChessGame
https://github.com/dchhetri/SFML-Chess/tree/master/ChessGame
*/

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <iostream>
#include <vector>
#include <array>

bool program_running = true;	// Program loop
bool playing = true;			// Spiller loop
bool gameover = false;			// Endscreen loop
bool blacklost = false;			// Sort har tabt
bool whitelost = false;			// Hvid har tabt
bool turn_variable = true;		// Tur variabel - True er hvid, false er sort

class Textures {
public:
	static void loadTextures();

	static sf::Texture blackTile;
	static sf::Texture whiteTile;

	static sf::Texture tileHighlight_PIECE;
	static sf::Texture tileHighlight_MOVE;
	static sf::Texture tileHighlight_ENEMY;

	static sf::Texture whitePawn;
	static sf::Texture whiteRook;
	static sf::Texture whiteBishop;
	static sf::Texture whiteKnight;
	static sf::Texture whiteQueen;
	static sf::Texture whiteKing;

	static sf::Texture blackPawn;
	static sf::Texture blackRook;
	static sf::Texture blackBishop;
	static sf::Texture blackKnight;
	static sf::Texture blackQueen;
	static sf::Texture blackKing;

	static sf::Texture endscreen_whitelost;
	static sf::Texture endscreen_blacklost;
};
/*---Class--Definitioner---*/
sf::Texture Textures::blackTile;
sf::Texture Textures::whiteTile;

sf::Texture Textures::tileHighlight_PIECE;
sf::Texture Textures::tileHighlight_MOVE;
sf::Texture Textures::tileHighlight_ENEMY;

sf::Texture Textures::whitePawn;
sf::Texture Textures::whiteRook;
sf::Texture Textures::whiteBishop;
sf::Texture Textures::whiteKnight;
sf::Texture Textures::whiteQueen;
sf::Texture Textures::whiteKing;

sf::Texture Textures::blackPawn;
sf::Texture Textures::blackRook;
sf::Texture Textures::blackBishop;
sf::Texture Textures::blackKnight;
sf::Texture Textures::blackQueen;
sf::Texture Textures::blackKing;

sf::Texture Textures::endscreen_whitelost;
sf::Texture Textures::endscreen_blacklost;
/*-------------------------*/

void Textures::loadTextures() {
	if (!blackTile.loadFromFile("Images/blackTile.png")		||
		!whiteTile.loadFromFile("Images/whiteTile.png")		||
		!tileHighlight_PIECE.loadFromFile("Images/tileHighlight_PIECE.png")	||
		!tileHighlight_MOVE.loadFromFile("Images/tileHighlight_MOVE.png")	||
		!tileHighlight_ENEMY.loadFromFile("Images/tileHighlight_ENEMY.png") ||
		!whitePawn.loadFromFile("Images/whitePawn.png")		||
		!whiteRook.loadFromFile("Images/whiteRook.png")		||
		!whiteBishop.loadFromFile("Images/whiteBishop.png") ||
		!whiteKnight.loadFromFile("Images/whiteKnight.png") ||
		!whiteQueen.loadFromFile("Images/whiteQueen.png")	||
		!whiteKing.loadFromFile("Images/whiteKing.png")		||
		!blackPawn.loadFromFile("Images/blackPawn.png")		||
		!blackRook.loadFromFile("Images/blackRook.png")		||
		!blackBishop.loadFromFile("Images/blackBishop.png") ||
		!blackKnight.loadFromFile("Images/blackKnight.png") ||
		!blackQueen.loadFromFile("Images/blackQueen.png")	||
		!blackKing.loadFromFile("Images/blackKing.png")		||
		!endscreen_whitelost.loadFromFile("Images/whiteLost.png") ||
		!endscreen_blacklost.loadFromFile("Images/blackLost.png")) {
			return;
	}
}

class ChessBoard {
public:
	static void _initialize();
	static void newGame();
	static void draw(sf::RenderWindow& window);
	static void clearHighlights();
	static void onClick(int col, int row);
	static bool gameOver();

	const enum TileHighlight { EMPTY, PIECE, MOVE, ENEMY };
	const enum TilePiece { NONE,
					 whitePAWN, whiteROOK, whiteKNIGHT, 
					 whiteBISHOP, whiteQUEEN, whiteKING,
					 blackPAWN, blackROOK, blackKNIGHT, 
					 blackBISHOP, blackQUEEN, blackKING	
	};

	static void movePiece(int new_x, int new_y, TilePiece& piece);

	struct TileSlot {
		sf::Sprite tileSprite;
		TilePiece tilePiece;
		TileHighlight tileHighlight;
	};

	typedef std::array< std::array<TileSlot*, 8>, 8 > TileType;
	static TileType tileBoard;
};
/*---Class--Definitioner---*/
ChessBoard::TileType ChessBoard::tileBoard = ChessBoard::TileType();
/*-------------------------*/

class Pawn {
public:
	static void getMoveLocations(int y, int x, const ChessBoard::TilePiece& piece);
};

class Rook {
public:
	static void getMoveLocations(int y, int x, const ChessBoard::TilePiece& piece);
};

class Knight {
public:
	static void getMoveLocations(int y, int x, const ChessBoard::TilePiece& piece);
};

class Bishop {
public:
	static void getMoveLocations(int y, int x, const ChessBoard::TilePiece& piece);
};

class Queen {
public:
	static void getMoveLocations(int y, int x, const ChessBoard::TilePiece& piece);
};

class King {
public:
	static void getMoveLocations(int y, int x, const ChessBoard::TilePiece& piece);
};

void ChessBoard::_initialize() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			tileBoard[i][j] = new TileSlot();
			
			if ((i + j) % 2 == 0) {
				tileBoard[i][j]->tileSprite.setTexture(Textures::whiteTile);
			}
			else {
				tileBoard[i][j]->tileSprite.setTexture(Textures::blackTile);
			}

			tileBoard[i][j]->tileSprite.setPosition(128 * j, 96 * i);
			tileBoard[i][j]->tileSprite.setScale(128, 96);
			tileBoard[i][j]->tileHighlight = EMPTY;
		}
	}
}

void ChessBoard::newGame() {
	/*---Clear--All---*/
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			tileBoard[i][j]->tilePiece = NONE;
			tileBoard[i][j]->tileHighlight = EMPTY;
		}
	}

	//---------------------------------------------
	
	/*---Pawns---*/
	for (int i = 0; i < 8; i++) {
		tileBoard[6][i]->tilePiece = whitePAWN;
		tileBoard[1][i]->tilePiece = blackPAWN;
	}

	/*---White--Rooks---*/
	tileBoard[7][0]->tilePiece = whiteROOK;
	tileBoard[7][7]->tilePiece = whiteROOK;
	
	/*---White--Knights---*/
	tileBoard[7][1]->tilePiece = whiteKNIGHT;
	tileBoard[7][6]->tilePiece = whiteKNIGHT;

	/*---White--Bishops---*/
	tileBoard[7][2]->tilePiece = whiteBISHOP;
	tileBoard[7][5]->tilePiece = whiteBISHOP;

	/*---White--Queen---*/
	tileBoard[7][3]->tilePiece = whiteQUEEN;

	/*---White--King---*/
	tileBoard[7][4]->tilePiece = whiteKING;
	//---------------------------------------------
	/*---Black--Rooks---*/
	tileBoard[0][0]->tilePiece = blackROOK;
	tileBoard[0][7]->tilePiece = blackROOK;

	/*---Black--Knights---*/
	tileBoard[0][1]->tilePiece = blackKNIGHT;
	tileBoard[0][6]->tilePiece = blackKNIGHT;

	/*---Black--Bishops---*/
	tileBoard[0][2]->tilePiece = blackBISHOP;
	tileBoard[0][5]->tilePiece = blackBISHOP;

	/*---Black--Queen---*/
	tileBoard[0][3]->tilePiece = blackQUEEN;

	/*---Black--King---*/
	tileBoard[0][4]->tilePiece = blackKING;

	clearHighlights();
	turn_variable = true;
}

void ChessBoard::draw(sf::RenderWindow& window) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			// Tegner highlights på boardet
			if (tileBoard[i][j]->tileHighlight != EMPTY) {
				sf::Sprite highlight;

				if (tileBoard[i][j]->tileHighlight == PIECE) {
					tileBoard[i][j]->tileSprite.setTexture(Textures::tileHighlight_PIECE);
				}
				else if (tileBoard[i][j]->tileHighlight == MOVE) {
					tileBoard[i][j]->tileSprite.setTexture(Textures::tileHighlight_MOVE);
				}
				else if (tileBoard[i][j]->tileHighlight == ENEMY) {
					tileBoard[i][j]->tileSprite.setTexture(Textures::tileHighlight_ENEMY);
				}
			}
			// Ellers tegner den bare farven igen
			else {
				if ((i + j) % 2 == 0) {
					tileBoard[i][j]->tileSprite.setTexture(Textures::whiteTile);
				}
				else {
					tileBoard[i][j]->tileSprite.setTexture(Textures::blackTile);
				}
			}
			window.draw(tileBoard[i][j]->tileSprite); // Renderer det
			
			// Tegner brikkerne
			if (tileBoard[i][j]->tilePiece != NONE) {
				sf::Sprite piece;

				if (tileBoard[i][j]->tilePiece == whitePAWN) {
					piece.setTexture(Textures::whitePawn);
				}
				else if (tileBoard[i][j]->tilePiece == whiteROOK) {
					piece.setTexture(Textures::whiteRook);
				}
				else if (tileBoard[i][j]->tilePiece == whiteKNIGHT) {
					piece.setTexture(Textures::whiteKnight);
				}
				else if (tileBoard[i][j]->tilePiece == whiteBISHOP) {
					piece.setTexture(Textures::whiteBishop);
				}
				else if (tileBoard[i][j]->tilePiece == whiteQUEEN) {
					piece.setTexture(Textures::whiteQueen);
				}
				else if (tileBoard[i][j]->tilePiece == whiteKING) {
					piece.setTexture(Textures::whiteKing);
				}
				else if (tileBoard[i][j]->tilePiece == blackPAWN) {
					piece.setTexture(Textures::blackPawn);
				}
				else if (tileBoard[i][j]->tilePiece == blackROOK) {
					piece.setTexture(Textures::blackRook);
				}
				else if (tileBoard[i][j]->tilePiece == blackKNIGHT) {
					piece.setTexture(Textures::blackKnight);
				}
				else if (tileBoard[i][j]->tilePiece == blackBISHOP) {
					piece.setTexture(Textures::blackBishop);
				}
				else if (tileBoard[i][j]->tilePiece == blackQUEEN) {
					piece.setTexture(Textures::blackQueen);
				}
				else if (tileBoard[i][j]->tilePiece == blackKING) {
					piece.setTexture(Textures::blackKing);
				}

				piece.setPosition(j * 128, i * 96); // Sætter den på det rigtige felt
				window.draw(piece); // Og renderer det
			}
		}
	}
}

void ChessBoard::clearHighlights() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			tileBoard[i][j]->tileHighlight = EMPTY;
		}
	}
}

void ChessBoard::onClick(int col, int row) {
	int old_x; // Nødvendig for at flytte den gamle brik
	int old_y; // Samme med den her
	
	// Man har trykket på en highlightet brik
	if (tileBoard[row][col]->tileHighlight != EMPTY) {
		// Clearer highlights hvis der trykkes på en der allerede er highlightet
		if (tileBoard[row][col]->tileHighlight == PIECE) {
			clearHighlights();
		}
		// Flytter highlightet brik til mulige sted og slutter turen
		else if (tileBoard[row][col]->tileHighlight == MOVE ||
			tileBoard[row][col]->tileHighlight == ENEMY) {
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					if (tileBoard[i][j]->tileHighlight == PIECE) {
						old_x = j;
						old_y = i;
					}
				}
			}
			movePiece(col, row, tileBoard[old_y][old_x]->tilePiece);
			
			if (turn_variable == true) {
				turn_variable = false; // Skifter turen til sort
			}
			else {
				turn_variable = true; // Skifter turen til hvid
			}
			
			if (gameOver()) {
				playing = false;
				gameover = true;
			}
		}
	}
	// Man har trykket på en brik
	else if (tileBoard[row][col]->tilePiece > 0) {
		if (turn_variable == true) {
			// Tjekker om tilePiece er hvid (de ligger fra i 1 til 6)
			if ((tileBoard[row][col]->tilePiece >= 1) && (tileBoard[row][col]->tilePiece <= 6)) {
				clearHighlights();
				tileBoard[row][col]->tileHighlight = PIECE;

				// Får moveset for brikken
				if (tileBoard[row][col]->tilePiece == whitePAWN) {
					Pawn::getMoveLocations(row, col, whitePAWN);
				}
				else if (tileBoard[row][col]->tilePiece == whiteROOK) {
					Rook::getMoveLocations(row, col, whiteROOK);
				}
				else if (tileBoard[row][col]->tilePiece == whiteKNIGHT) {
					Knight::getMoveLocations(row, col, whiteKNIGHT);
				}
				else if (tileBoard[row][col]->tilePiece == whiteBISHOP) {
					Bishop::getMoveLocations(row, col, whiteBISHOP);
				}
				else if (tileBoard[row][col]->tilePiece == whiteQUEEN) {
					Queen::getMoveLocations(row, col, whiteQUEEN);
				}
				else if (tileBoard[row][col]->tilePiece == whiteKING) {
					King::getMoveLocations(row, col, whiteKING);
				}
			}
		}
		else if (turn_variable == false) {
			// Tjekker om tilePiece er sort (de ligger fra 7 op til 12)
			if ((tileBoard[row][col]->tilePiece >= 7) && (tileBoard[row][col]->tilePiece <= 12)) {
				clearHighlights();
				tileBoard[row][col]->tileHighlight = PIECE;

				// Får moveset for brikken
				if (tileBoard[row][col]->tilePiece == blackPAWN) {
					Pawn::getMoveLocations(row, col, blackPAWN);
				}
				else if (tileBoard[row][col]->tilePiece == blackROOK) {
					Rook::getMoveLocations(row, col, blackROOK);
				}
				else if (tileBoard[row][col]->tilePiece == blackKNIGHT) {
					Knight::getMoveLocations(row, col, blackKNIGHT);
				}
				else if (tileBoard[row][col]->tilePiece == blackBISHOP) {
					Bishop::getMoveLocations(row, col, blackBISHOP);
				}
				else if (tileBoard[row][col]->tilePiece == blackQUEEN) {
					Queen::getMoveLocations(row, col, blackQUEEN);
				}
				else if (tileBoard[row][col]->tilePiece == blackKING) {
					King::getMoveLocations(row, col, blackKING);
				}
			}
		}
	}
	// Man har trykket på ingenting.. Clearer alle highlights
	else {
		clearHighlights();
	}
}

void ChessBoard::movePiece(int new_x, int new_y, TilePiece& piece) {
	tileBoard[new_y][new_x]->tilePiece = piece;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (tileBoard[i][j]->tileHighlight == PIECE) {
				tileBoard[i][j]->tilePiece = NONE;
			}
		}
	}
	clearHighlights();
}

bool ChessBoard::gameOver() {
	bool found_whiteKing = false;
	bool found_blackKing = false;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (tileBoard[i][j]->tilePiece == whiteKING) {
				found_whiteKing = true;
			}
			else if (tileBoard[i][j]->tilePiece == blackKING) {
				found_blackKing = true;
			}
		}
	}

	if (!found_whiteKing) {
		whitelost = true;
		return true;
	}
	else if (!found_blackKing) {
		blacklost = true;
		return true;
	}
	else {
		return false;
	}
}

void Pawn::getMoveLocations(int i, int j, const ChessBoard::TilePiece& piece) {
	// White Pawn
	if (piece == ChessBoard::whitePAWN) {
		// Tjekker om Pawn overhovedet har flyttet endnu
		if (i > 0) {
			if (ChessBoard::tileBoard[i - 1][j]->tilePiece == ChessBoard::TilePiece::NONE) {
				// Hvid bonde har ikke rykket endnu
				if (i == 6) {
					if (ChessBoard::tileBoard[i - 2][j]->tilePiece == ChessBoard::TilePiece::NONE)
					ChessBoard::tileBoard[i - 2][j]->tileHighlight = ChessBoard::TileHighlight::MOVE;
				}

				// Hvid kan rykke en gang op når der ikke er modstandere
				ChessBoard::tileBoard[i - 1][j]->tileHighlight = ChessBoard::TileHighlight::MOVE;
			}
			
			// Tjekker om den kan tage nogle modstanderbrikker
			if (j > 0) {
				if (ChessBoard::tileBoard[i - 1][j - 1]->tilePiece >= 7 &&
					ChessBoard::tileBoard[i - 1][j - 1]->tilePiece <= 12) {
					ChessBoard::tileBoard[i - 1][j - 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
				}
			}

			if (j < 7) {
				if (ChessBoard::tileBoard[i - 1][j + 1]->tilePiece >= 7 &&
					ChessBoard::tileBoard[i - 1][j + 1]->tilePiece <= 12) {
					ChessBoard::tileBoard[i - 1][j + 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
				}
			}
		}
	}
	// Black Pawn
	else if (piece == ChessBoard::blackPAWN) {
		// Tjekker om Pawn overhovedet har flyttet endnu
		if (i < 7) {
			if (ChessBoard::tileBoard[i + 1][j]->tilePiece == ChessBoard::TilePiece::NONE) {
				// Hvid bonde har ikke rykket endnu
				if (i == 1) {
					if (ChessBoard::tileBoard[i + 2][j]->tilePiece == ChessBoard::TilePiece::NONE)
						ChessBoard::tileBoard[i + 2][j]->tileHighlight = ChessBoard::TileHighlight::MOVE;
				}

				// Hvid kan rykke en gang op når der ikke er modstandere
				ChessBoard::tileBoard[i + 1][j]->tileHighlight = ChessBoard::TileHighlight::MOVE;
			}

			// Tjekker om den kan tage nogle modstanderbrikker
			if (j > 0) {
				if (ChessBoard::tileBoard[i + 1][j - 1]->tilePiece >= 1 &&
					ChessBoard::tileBoard[i + 1][j - 1]->tilePiece <= 6) {
					ChessBoard::tileBoard[i + 1][j - 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
				}
			}

			if (j < 7) {
				if (ChessBoard::tileBoard[i + 1][j + 1]->tilePiece >= 1 &&
					ChessBoard::tileBoard[i + 1][j + 1]->tilePiece <= 6) {
					ChessBoard::tileBoard[i + 1][j + 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
				}
			}
		}
	}
}

void Rook::getMoveLocations(int i, int j, const ChessBoard::TilePiece& piece) {
	// Loop variabel
	bool checking = true;

	// Tjekker over tårnet
	int count = i;
	while (checking) {
		if (count > 0) {
			if (ChessBoard::tileBoard[count - 1][j]->tilePiece == ChessBoard::TilePiece::NONE) {
				ChessBoard::tileBoard[count - 1][j]->tileHighlight = ChessBoard::TileHighlight::MOVE;
				count--;
			}
			else if (piece == ChessBoard::whiteROOK) {
				// Tjekker hvid tårn for fjender
				if (ChessBoard::tileBoard[count - 1][j]->tilePiece >= 7 &&
					ChessBoard::tileBoard[count - 1][j]->tilePiece <= 12) {
						ChessBoard::tileBoard[count - 1][j]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
				}
				checking = false;
			}
			else if (piece == ChessBoard::blackROOK) {
				// Tjekker sort tårn for fjender
				if (ChessBoard::tileBoard[count - 1][j]->tilePiece >= 1 &&
					ChessBoard::tileBoard[count - 1][j]->tilePiece <= 6) {
						ChessBoard::tileBoard[count - 1][j]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
				}
				checking = false;
			}
			else {
				checking = false;
			}
		}
		else {
			checking = false;
		}
	}

	// Tjekker under tårnet
	count = i;
	checking = true;
	while (checking) {
		if (count < 7) {
			if (ChessBoard::tileBoard[count + 1][j]->tilePiece == ChessBoard::TilePiece::NONE) {
				ChessBoard::tileBoard[count + 1][j]->tileHighlight = ChessBoard::TileHighlight::MOVE;
				count++;
			}
			else if (piece == ChessBoard::whiteROOK) {
				// Tjekker hvid tårn for fjender
				if (ChessBoard::tileBoard[count + 1][j]->tilePiece >= 7 &&
					ChessBoard::tileBoard[count + 1][j]->tilePiece <= 12) {
						ChessBoard::tileBoard[count + 1][j]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
				}
				checking = false;
			}
			else if (piece == ChessBoard::blackROOK) {
				// Tjekker sort tårn for fjender
				if (ChessBoard::tileBoard[count + 1][j]->tilePiece >= 1 &&
					ChessBoard::tileBoard[count + 1][j]->tilePiece <= 6) {
						ChessBoard::tileBoard[count + 1][j]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
				}
				checking = false;
			}
			else {
				checking = false;
			}
		}
		else {
			checking = false;
		}
	}

	// Tjekker til venstre for tårnet
	count = j;
	checking = true;
	while (checking) {
		if (count > 0) {
			if (ChessBoard::tileBoard[i][count - 1]->tilePiece == ChessBoard::TilePiece::NONE) {
				ChessBoard::tileBoard[i][count - 1]->tileHighlight = ChessBoard::TileHighlight::MOVE;
				count--;
			}
			else if (piece == ChessBoard::whiteROOK) {
				// Tjekker hvid tårn for fjender
				if (ChessBoard::tileBoard[i][count - 1]->tilePiece >= 7 &&
					ChessBoard::tileBoard[i][count - 1]->tilePiece <= 12) {
						ChessBoard::tileBoard[i][count - 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
				}
				checking = false;
			}
			else if (piece == ChessBoard::blackROOK) {
				// Tjekker sort tårn for fjender
				if (ChessBoard::tileBoard[i][count - 1]->tilePiece >= 1 &&
					ChessBoard::tileBoard[i][count - 1]->tilePiece <= 6) {
						ChessBoard::tileBoard[i][count - 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
				}
				checking = false;
			}
			else {
				checking = false;
			}
		}
		else {
			checking = false;
		}
	}

	// Tjekker til højre for tårnet
	count = j;
	checking = true;
	while (checking) {
		if (count < 7) {
			if (ChessBoard::tileBoard[i][count + 1]->tilePiece == ChessBoard::TilePiece::NONE) {
				ChessBoard::tileBoard[i][count + 1]->tileHighlight = ChessBoard::TileHighlight::MOVE;
				count++;
			}
			else if (piece == ChessBoard::whiteROOK) {
				// Tjekker hvid tårn for fjender
				if (ChessBoard::tileBoard[i][count + 1]->tilePiece >= 7 &&
					ChessBoard::tileBoard[i][count + 1]->tilePiece <= 12) {
						ChessBoard::tileBoard[i][count + 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
				}
				checking = false;
			}
			else if (piece == ChessBoard::blackROOK) {
				// Tjekker sort tårn for fjender
				if (ChessBoard::tileBoard[i][count + 1]->tilePiece >= 1 &&
					ChessBoard::tileBoard[i][count + 1]->tilePiece <= 6) {
						ChessBoard::tileBoard[i][count + 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
				}
				checking = false;
			}
			else {
				checking = false;
			}
		}
		else {
			checking = false;
		}
	}
}

void Knight::getMoveLocations(int i, int j, const ChessBoard::TilePiece& piece) {
	// 8 forskellige flytmuligheder, da den kan gå i 4 retninger
	// og derefter vælge 2 retninger. Vi tager hver mulighed for sig

	// 2 op, 1 til venstre
	if (i > 2 && j > 0) {
		if (ChessBoard::tileBoard[i - 2][j - 1]->tilePiece == ChessBoard::TilePiece::NONE) {
			ChessBoard::tileBoard[i - 2][j - 1]->tileHighlight = ChessBoard::TileHighlight::MOVE;
		}
		else if (piece == ChessBoard::whiteKNIGHT) {
			if (ChessBoard::tileBoard[i - 2][j - 1]->tilePiece >= 7 &&
				ChessBoard::tileBoard[i - 2][j - 1]->tilePiece <= 12) {
				ChessBoard::tileBoard[i - 2][j - 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
		else if (piece == ChessBoard::blackKNIGHT) {
			if (ChessBoard::tileBoard[i - 2][j - 1]->tilePiece >= 1 &&
				ChessBoard::tileBoard[i - 2][j - 1]->tilePiece <= 6) {
				ChessBoard::tileBoard[i - 2][j - 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
	}

	// 2 op, 1 til højre
	if (i > 1 && j < 7) {
		if (ChessBoard::tileBoard[i - 2][j + 1]->tilePiece == ChessBoard::TilePiece::NONE) {
			ChessBoard::tileBoard[i - 2][j + 1]->tileHighlight = ChessBoard::TileHighlight::MOVE;
		}
		else if (piece == ChessBoard::whiteKNIGHT) {
			if (ChessBoard::tileBoard[i - 2][j + 1]->tilePiece >= 7 &&
				ChessBoard::tileBoard[i - 2][j + 1]->tilePiece <= 12) {
				ChessBoard::tileBoard[i - 2][j + 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
		else if (piece == ChessBoard::blackKNIGHT) {
			if (ChessBoard::tileBoard[i - 2][j + 1]->tilePiece >= 1 &&
				ChessBoard::tileBoard[i - 2][j + 1]->tilePiece <= 6) {
				ChessBoard::tileBoard[i - 2][j + 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
	}

	// 2 til højre, 1 op
	if (j < 6 && i > 0) {
		if (ChessBoard::tileBoard[i - 1][j + 2]->tilePiece == ChessBoard::TilePiece::NONE) {
			ChessBoard::tileBoard[i - 1][j + 2]->tileHighlight = ChessBoard::TileHighlight::MOVE;
		}
		else if (piece == ChessBoard::whiteKNIGHT) {
			if (ChessBoard::tileBoard[i - 1][j + 2]->tilePiece >= 7 &&
				ChessBoard::tileBoard[i - 1][j + 2]->tilePiece <= 12) {
				ChessBoard::tileBoard[i - 1][j + 2]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
		else if (piece == ChessBoard::blackKNIGHT) {
			if (ChessBoard::tileBoard[i - 1][j + 2]->tilePiece >= 1 &&
				ChessBoard::tileBoard[i - 1][j + 2]->tilePiece <= 6) {
				ChessBoard::tileBoard[i - 1][j + 2]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
	}

	// 2 til højre, 1 ned
	if (j < 6 && i < 7) {
		if (ChessBoard::tileBoard[i + 1][j + 2]->tilePiece == ChessBoard::TilePiece::NONE) {
			ChessBoard::tileBoard[i + 1][j + 2]->tileHighlight = ChessBoard::TileHighlight::MOVE;
		}
		else if (piece == ChessBoard::whiteKNIGHT) {
			if (ChessBoard::tileBoard[i + 1][j + 2]->tilePiece >= 7 &&
				ChessBoard::tileBoard[i + 1][j + 2]->tilePiece <= 12) {
				ChessBoard::tileBoard[i + 1][j + 2]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
		else if (piece == ChessBoard::blackKNIGHT) {
			if (ChessBoard::tileBoard[i + 1][j + 2]->tilePiece >= 1 &&
				ChessBoard::tileBoard[i + 1][j + 2]->tilePiece <= 6) {
				ChessBoard::tileBoard[i + 1][j + 2]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
	}

	// 2 ned, 1 til højre
	if (i < 6 && j < 7) {
		if (ChessBoard::tileBoard[i + 2][j + 1]->tilePiece == ChessBoard::TilePiece::NONE) {
			ChessBoard::tileBoard[i + 2][j + 1]->tileHighlight = ChessBoard::TileHighlight::MOVE;
		}
		else if (piece == ChessBoard::whiteKNIGHT) {
			if (ChessBoard::tileBoard[i + 2][j + 1]->tilePiece >= 7 &&
				ChessBoard::tileBoard[i + 2][j + 1]->tilePiece <= 12) {
				ChessBoard::tileBoard[i + 2][j + 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
		else if (piece == ChessBoard::blackKNIGHT) {
			if (ChessBoard::tileBoard[i + 2][j + 1]->tilePiece >= 1 &&
				ChessBoard::tileBoard[i + 2][j + 1]->tilePiece <= 6) {
				ChessBoard::tileBoard[i + 2][j + 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
	}

	// 2 ned, 1 til venstre
	if (i < 6 && j > 0) {
		if (ChessBoard::tileBoard[i + 2][j - 1]->tilePiece == ChessBoard::TilePiece::NONE) {
			ChessBoard::tileBoard[i + 2][j - 1]->tileHighlight = ChessBoard::TileHighlight::MOVE;
		}
		else if (piece == ChessBoard::whiteKNIGHT) {
			if (ChessBoard::tileBoard[i + 2][j - 1]->tilePiece >= 7 &&
				ChessBoard::tileBoard[i + 2][j - 1]->tilePiece <= 12) {
				ChessBoard::tileBoard[i + 2][j - 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
		else if (piece == ChessBoard::blackKNIGHT) {
			if (ChessBoard::tileBoard[i + 2][j - 1]->tilePiece >= 1 &&
				ChessBoard::tileBoard[i + 2][j - 1]->tilePiece <= 6) {
				ChessBoard::tileBoard[i + 2][j - 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
	}

	// 2 til venstre, 1 ned
	if (j > 1 && i < 7) {
		if (ChessBoard::tileBoard[i + 1][j - 2]->tilePiece == ChessBoard::TilePiece::NONE) {
			ChessBoard::tileBoard[i + 1][j - 2]->tileHighlight = ChessBoard::TileHighlight::MOVE;
		}
		else if (piece == ChessBoard::whiteKNIGHT) {
			if (ChessBoard::tileBoard[i + 1][j - 2]->tilePiece >= 7 &&
				ChessBoard::tileBoard[i + 1][j - 2]->tilePiece <= 12) {
				ChessBoard::tileBoard[i + 1][j - 2]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
		else if (piece == ChessBoard::blackKNIGHT) {
			if (ChessBoard::tileBoard[i + 1][j - 2]->tilePiece >= 1 &&
				ChessBoard::tileBoard[i + 1][j - 2]->tilePiece <= 6) {
				ChessBoard::tileBoard[i + 1][j - 2]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
	}

	// 2 til venstre, 1 op
	if (j > 1 && i > 0) {
		if (ChessBoard::tileBoard[i - 1][j - 2]->tilePiece == ChessBoard::TilePiece::NONE) {
			ChessBoard::tileBoard[i - 1][j - 2]->tileHighlight = ChessBoard::TileHighlight::MOVE;
		}
		else if (piece == ChessBoard::whiteKNIGHT) {
			if (ChessBoard::tileBoard[i - 1][j - 2]->tilePiece >= 7 &&
				ChessBoard::tileBoard[i - 1][j - 2]->tilePiece <= 12) {
				ChessBoard::tileBoard[i - 1][j - 2]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
		else if (piece == ChessBoard::blackKNIGHT) {
			if (ChessBoard::tileBoard[i - 1][j - 2]->tilePiece >= 1 &&
				ChessBoard::tileBoard[i - 1][j - 2]->tilePiece <= 6) {
				ChessBoard::tileBoard[i - 1][j - 2]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
	}
}

void Bishop::getMoveLocations(int i, int j, const ChessBoard::TilePiece& piece) {
	// Loop variabel
	bool checking = true;

	// Tjekker oppe til højre først
	int count_i = i;
	int count_j = j;
	while (checking) {
		if (count_i > 0 && count_j < 7) {
			if (ChessBoard::tileBoard[count_i - 1][count_j + 1]->tilePiece == ChessBoard::TilePiece::NONE) {
				ChessBoard::tileBoard[count_i - 1][count_j + 1]->tileHighlight = ChessBoard::TileHighlight::MOVE;
				count_i--;
				count_j++;
			}
			else if (piece == ChessBoard::whiteBISHOP) {
				// Tjekker hvid tårn for fjender
				if (ChessBoard::tileBoard[count_i - 1][count_j + 1]->tilePiece >= 7 &&
					ChessBoard::tileBoard[count_i - 1][count_j + 1]->tilePiece <= 12) {
					ChessBoard::tileBoard[count_i - 1][count_j + 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
				}
				checking = false;
			}
			else if (piece == ChessBoard::blackBISHOP) {
				// Tjekker sort tårn for fjender
				if (ChessBoard::tileBoard[count_i - 1][count_j + 1]->tilePiece >= 1 &&
					ChessBoard::tileBoard[count_i - 1][count_j + 1]->tilePiece <= 6) {
					ChessBoard::tileBoard[count_i - 1][count_j + 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
				}
				checking = false;
			}
			else {
				checking = false;
			}
		}
		else {
			checking = false;
		}
	}

	// Tjekker oppe til venstre bagefter
	count_i = i;
	count_j = j;
	checking = true;

	while (checking) {
		if (count_i > 0 && count_j > 0) {
			if (ChessBoard::tileBoard[count_i - 1][count_j - 1]->tilePiece == ChessBoard::TilePiece::NONE) {
				ChessBoard::tileBoard[count_i - 1][count_j - 1]->tileHighlight = ChessBoard::TileHighlight::MOVE;
				count_i--;
				count_j--;
			}
			else if (piece == ChessBoard::whiteBISHOP) {
				// Tjekker hvid tårn for fjender
				if (ChessBoard::tileBoard[count_i - 1][count_j - 1]->tilePiece >= 7 &&
					ChessBoard::tileBoard[count_i - 1][count_j - 1]->tilePiece <= 12) {
					ChessBoard::tileBoard[count_i - 1][count_j - 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
				}
				checking = false;
			}
			else if (piece == ChessBoard::blackBISHOP) {
				// Tjekker sort tårn for fjender
				if (ChessBoard::tileBoard[count_i - 1][count_j - 1]->tilePiece >= 1 &&
					ChessBoard::tileBoard[count_i - 1][count_j - 1]->tilePiece <= 6) {
					ChessBoard::tileBoard[count_i - 1][count_j - 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
				}
				checking = false;
			}
			else {
				checking = false;
			}
		}
		else {
			checking = false;
		}
	}

	// Tjekker nede til venstre
	count_i = i;
	count_j = j;
	checking = true;

	while (checking) {
		if (count_i < 7 && count_j > 0) {
			if (ChessBoard::tileBoard[count_i + 1][count_j - 1]->tilePiece == ChessBoard::TilePiece::NONE) {
				ChessBoard::tileBoard[count_i + 1][count_j - 1]->tileHighlight = ChessBoard::TileHighlight::MOVE;
				count_i++;
				count_j--;
			}
			else if (piece == ChessBoard::whiteBISHOP) {
				// Tjekker hvid tårn for fjender
				if (ChessBoard::tileBoard[count_i + 1][count_j - 1]->tilePiece >= 7 &&
					ChessBoard::tileBoard[count_i + 1][count_j - 1]->tilePiece <= 12) {
					ChessBoard::tileBoard[count_i + 1][count_j - 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
				}
				checking = false;
			}
			else if (piece == ChessBoard::blackBISHOP) {
				// Tjekker sort tårn for fjender
				if (ChessBoard::tileBoard[count_i + 1][count_j - 1]->tilePiece >= 1 &&
					ChessBoard::tileBoard[count_i + 1][count_j - 1]->tilePiece <= 6) {
					ChessBoard::tileBoard[count_i + 1][count_j - 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
				}
				checking = false;
			}
			else {
				checking = false;
			}
		}
		else {
			checking = false;
		}
	}

	// Tjekker nede til højre til sidst
	count_i = i;
	count_j = j;
	checking = true;

	while (checking) {
		if (count_i < 7 && count_j < 7) {
			if (ChessBoard::tileBoard[count_i + 1][count_j + 1]->tilePiece == ChessBoard::TilePiece::NONE) {
				ChessBoard::tileBoard[count_i + 1][count_j + 1]->tileHighlight = ChessBoard::TileHighlight::MOVE;
				count_i++;
				count_j++;
			}
			else if (piece == ChessBoard::whiteBISHOP) {
				// Tjekker hvid tårn for fjender
				if (ChessBoard::tileBoard[count_i + 1][count_j + 1]->tilePiece >= 7 &&
					ChessBoard::tileBoard[count_i + 1][count_j + 1]->tilePiece <= 12) {
					ChessBoard::tileBoard[count_i + 1][count_j + 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
				}
				checking = false;
			}
			else if (piece == ChessBoard::blackBISHOP) {
				// Tjekker sort tårn for fjender
				if (ChessBoard::tileBoard[count_i + 1][count_j + 1]->tilePiece >= 1 &&
					ChessBoard::tileBoard[count_i + 1][count_j + 1]->tilePiece <= 6) {
					ChessBoard::tileBoard[count_i + 1][count_j + 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
				}
				checking = false;
			}
			else {
				checking = false;
			}
		}
		else {
			checking = false;
		}
	}
}

void Queen::getMoveLocations(int i, int j, const ChessBoard::TilePiece& piece) {
	// Dronningen skal tjekke ligesom tårnet og løberen
	// Og derfor kan vi egentlig bare kalde de funktioner
	// Med dronningens farve, og få samme resultat

	if (piece == ChessBoard::whiteQUEEN) {
		Rook::getMoveLocations(i, j, ChessBoard::whiteROOK);
		Bishop::getMoveLocations(i, j, ChessBoard::whiteBISHOP);
	}
	else if (piece == ChessBoard::blackQUEEN) {
		Rook::getMoveLocations(i, j, ChessBoard::blackROOK);
		Bishop::getMoveLocations(i, j, ChessBoard::blackBISHOP);
	}
}

void King::getMoveLocations(int i, int j, const ChessBoard::TilePiece& piece) {
	// Op og ned
	// Ned
	if (i == 0) {
		if (ChessBoard::tileBoard[i + 1][j]->tilePiece == ChessBoard::TilePiece::NONE) {
			ChessBoard::tileBoard[i + 1][j]->tileHighlight = ChessBoard::TileHighlight::MOVE;
		}
		else if (piece == ChessBoard::whiteKING) {
			if (ChessBoard::tileBoard[i + 1][j]->tilePiece >= 7 &&
				ChessBoard::tileBoard[i + 1][j]->tilePiece <= 12) {
				ChessBoard::tileBoard[i + 1][j]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
		else if (piece == ChessBoard::blackKING) {
			if (ChessBoard::tileBoard[i + 1][j]->tilePiece >= 1 &&
				ChessBoard::tileBoard[i + 1][j]->tilePiece <= 6) {
				ChessBoard::tileBoard[i + 1][j]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
	}
	// Både op og ned
	else if (i > 0 && i < 7) {
		// Under
		if (ChessBoard::tileBoard[i + 1][j]->tilePiece == ChessBoard::TilePiece::NONE) {
			ChessBoard::tileBoard[i + 1][j]->tileHighlight = ChessBoard::TileHighlight::MOVE;
		}
		else if (piece == ChessBoard::whiteKING) {
			if (ChessBoard::tileBoard[i + 1][j]->tilePiece >= 7 &&
				ChessBoard::tileBoard[i + 1][j]->tilePiece <= 12) {
				ChessBoard::tileBoard[i + 1][j]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
		else if (piece == ChessBoard::blackKING) {
			if (ChessBoard::tileBoard[i + 1][j]->tilePiece >= 1 &&
				ChessBoard::tileBoard[i + 1][j]->tilePiece <= 6) {
				ChessBoard::tileBoard[i + 1][j]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
		// Over
		if (ChessBoard::tileBoard[i - 1][j]->tilePiece == ChessBoard::TilePiece::NONE) {
			ChessBoard::tileBoard[i - 1][j]->tileHighlight = ChessBoard::TileHighlight::MOVE;
		}
		else if (piece == ChessBoard::whiteKING) {
			if (ChessBoard::tileBoard[i - 1][j]->tilePiece >= 7 &&
				ChessBoard::tileBoard[i - 1][j]->tilePiece <= 12) {
				ChessBoard::tileBoard[i - 1][j]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
		else if (piece == ChessBoard::blackKING) {
			if (ChessBoard::tileBoard[i - 1][j]->tilePiece >= 1 &&
				ChessBoard::tileBoard[i - 1][j]->tilePiece <= 6) {
				ChessBoard::tileBoard[i - 1][j]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
	}
	// Op
	else if (i == 7) {
		if (ChessBoard::tileBoard[i - 1][j]->tilePiece == ChessBoard::TilePiece::NONE) {
			ChessBoard::tileBoard[i - 1][j]->tileHighlight = ChessBoard::TileHighlight::MOVE;
		}
		else if (piece == ChessBoard::whiteKING) {
			if (ChessBoard::tileBoard[i - 1][j]->tilePiece >= 7 &&
				ChessBoard::tileBoard[i - 1][j]->tilePiece <= 12) {
				ChessBoard::tileBoard[i - 1][j]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
		else if (piece == ChessBoard::blackKING) {
			if (ChessBoard::tileBoard[i - 1][j]->tilePiece >= 1 &&
				ChessBoard::tileBoard[i - 1][j]->tilePiece <= 6) {
				ChessBoard::tileBoard[i - 1][j]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
	}
	// Højre og venstre
	// Højre
	if (j == 0) {
		if (ChessBoard::tileBoard[i][j + 1]->tilePiece == ChessBoard::TilePiece::NONE) {
			ChessBoard::tileBoard[i][j + 1]->tileHighlight = ChessBoard::TileHighlight::MOVE;
		}
		else if (piece == ChessBoard::whiteKING) {
			if (ChessBoard::tileBoard[i][j + 1]->tilePiece >= 7 &&
				ChessBoard::tileBoard[i][j + 1]->tilePiece <= 12) {
				ChessBoard::tileBoard[i][j + 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
		else if (piece == ChessBoard::blackKING) {
			if (ChessBoard::tileBoard[i][j + 1]->tilePiece >= 1 &&
				ChessBoard::tileBoard[i][j + 1]->tilePiece <= 6) {
				ChessBoard::tileBoard[i][j + 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
	}
	// Højre og venstre
	else if (j > 0 && j < 7) {
		// Højre
		if (ChessBoard::tileBoard[i][j + 1]->tilePiece == ChessBoard::TilePiece::NONE) {
			ChessBoard::tileBoard[i][j + 1]->tileHighlight = ChessBoard::TileHighlight::MOVE;
		}
		else if (piece == ChessBoard::whiteKING) {
			if (ChessBoard::tileBoard[i][j + 1]->tilePiece >= 7 &&
				ChessBoard::tileBoard[i][j + 1]->tilePiece <= 12) {
				ChessBoard::tileBoard[i][j + 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
		else if (piece == ChessBoard::blackKING) {
			if (ChessBoard::tileBoard[i][j + 1]->tilePiece >= 1 &&
				ChessBoard::tileBoard[i][j + 1]->tilePiece <= 6) {
				ChessBoard::tileBoard[i][j + 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
		// Venstre
		if (ChessBoard::tileBoard[i][j - 1]->tilePiece == ChessBoard::TilePiece::NONE) {
			ChessBoard::tileBoard[i][j - 1]->tileHighlight = ChessBoard::TileHighlight::MOVE;
		}
		else if (piece == ChessBoard::whiteKING) {
			if (ChessBoard::tileBoard[i][j - 1]->tilePiece >= 7 &&
				ChessBoard::tileBoard[i][j - 1]->tilePiece <= 12) {
				ChessBoard::tileBoard[i][j - 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
		else if (piece == ChessBoard::blackKING) {
			if (ChessBoard::tileBoard[i][j - 1]->tilePiece >= 1 &&
				ChessBoard::tileBoard[i][j - 1]->tilePiece <= 6) {
				ChessBoard::tileBoard[i][j - 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
	}
	// Venstre
	else if (j == 7) {
		if (ChessBoard::tileBoard[i][j - 1]->tilePiece == ChessBoard::TilePiece::NONE) {
			ChessBoard::tileBoard[i][j - 1]->tileHighlight = ChessBoard::TileHighlight::MOVE;
		}
		else if (piece == ChessBoard::whiteKING) {
			if (ChessBoard::tileBoard[i][j - 1]->tilePiece >= 7 &&
				ChessBoard::tileBoard[i][j - 1]->tilePiece <= 12) {
				ChessBoard::tileBoard[i][j - 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
		else if (piece == ChessBoard::blackKING) {
			if (ChessBoard::tileBoard[i][j - 1]->tilePiece >= 1 &&
				ChessBoard::tileBoard[i][j - 1]->tilePiece <= 6) {
				ChessBoard::tileBoard[i][j - 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
	}
	// Oppe til venstre
	if (i > 0 && j > 0) {
		if (ChessBoard::tileBoard[i - 1][j - 1]->tilePiece == ChessBoard::TilePiece::NONE) {
			ChessBoard::tileBoard[i - 1][j - 1]->tileHighlight = ChessBoard::TileHighlight::MOVE;
		}
		else if (piece == ChessBoard::whiteKING) {
			if (ChessBoard::tileBoard[i - 1][j - 1]->tilePiece >= 7 &&
				ChessBoard::tileBoard[i - 1][j - 1]->tilePiece <= 12) {
				ChessBoard::tileBoard[i - 1][j - 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
		else if (piece == ChessBoard::blackKING) {
			if (ChessBoard::tileBoard[i - 1][j - 1]->tilePiece >= 1 &&
				ChessBoard::tileBoard[i - 1][j - 1]->tilePiece <= 6) {
				ChessBoard::tileBoard[i - 1][j - 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
	}
	// Nede til venstre
	if (i < 7 && j > 0) {
		if (ChessBoard::tileBoard[i + 1][j - 1]->tilePiece == ChessBoard::TilePiece::NONE) {
			ChessBoard::tileBoard[i + 1][j - 1]->tileHighlight = ChessBoard::TileHighlight::MOVE;
		}
		else if (piece == ChessBoard::whiteKING) {
			if (ChessBoard::tileBoard[i + 1][j - 1]->tilePiece >= 7 &&
				ChessBoard::tileBoard[i + 1][j - 1]->tilePiece <= 12) {
				ChessBoard::tileBoard[i + 1][j - 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
		else if (piece == ChessBoard::blackKING) {
			if (ChessBoard::tileBoard[i + 1][j - 1]->tilePiece >= 1 &&
				ChessBoard::tileBoard[i + 1][j - 1]->tilePiece <= 6) {
				ChessBoard::tileBoard[i + 1][j - 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
	}
	// Nede til højre
	if (i < 7 && j < 7) {
		if (ChessBoard::tileBoard[i + 1][j + 1]->tilePiece == ChessBoard::TilePiece::NONE) {
			ChessBoard::tileBoard[i + 1][j + 1]->tileHighlight = ChessBoard::TileHighlight::MOVE;
		}
		else if (piece == ChessBoard::whiteKING) {
			if (ChessBoard::tileBoard[i + 1][j + 1]->tilePiece >= 7 &&
				ChessBoard::tileBoard[i + 1][j + 1]->tilePiece <= 12) {
				ChessBoard::tileBoard[i + 1][j + 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
		else if (piece == ChessBoard::blackKING) {
			if (ChessBoard::tileBoard[i + 1][j + 1]->tilePiece >= 1 &&
				ChessBoard::tileBoard[i + 1][j + 1]->tilePiece <= 6) {
				ChessBoard::tileBoard[i + 1][j + 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
	}
	// Oppe til højre
	if (i > 0 && j < 7) {
		if (ChessBoard::tileBoard[i - 1][j + 1]->tilePiece == ChessBoard::TilePiece::NONE) {
			ChessBoard::tileBoard[i - 1][j + 1]->tileHighlight = ChessBoard::TileHighlight::MOVE;
		}
		else if (piece == ChessBoard::whiteKING) {
			if (ChessBoard::tileBoard[i - 1][j + 1]->tilePiece >= 7 &&
				ChessBoard::tileBoard[i - 1][j + 1]->tilePiece <= 12) {
				ChessBoard::tileBoard[i - 1][j + 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
		else if (piece == ChessBoard::blackKING) {
			if (ChessBoard::tileBoard[i - 1][j + 1]->tilePiece >= 1 &&
				ChessBoard::tileBoard[i - 1][j + 1]->tilePiece <= 6) {
				ChessBoard::tileBoard[i - 1][j + 1]->tileHighlight = ChessBoard::TileHighlight::ENEMY;
			}
		}
	}
}

int main(int argc, char* argv[]) {
	sf::RenderWindow renderWindow;
	renderWindow.create(sf::VideoMode(1024, 768), "Skak - Eksamensprojekt", (sf::Style::Close));
	renderWindow.setFramerateLimit(15);

	Textures::loadTextures();
	ChessBoard::_initialize();
	ChessBoard::newGame();

	// Programmet kører
	while (program_running) {
		// Spillet kører
		while (playing) {
			sf::Event currentEvent;
			while (renderWindow.pollEvent(currentEvent)) {

				if (currentEvent.type == sf::Event::Closed) {
					program_running = false;
					playing = false;
					renderWindow.close();
				}

				if (currentEvent.type == sf::Event::KeyPressed) {
					if (currentEvent.key.code == sf::Keyboard::Escape) {
						program_running = false;
						playing = false;
						renderWindow.close();
					}
					else if (currentEvent.key.code == sf::Keyboard::R) {
						ChessBoard::newGame();
					}
				}

				if (currentEvent.type == sf::Event::MouseButtonPressed) {
					if (currentEvent.mouseButton.button == sf::Mouse::Left) {
						if (std::floor(currentEvent.mouseButton.x / 128) >= 0 &&
							std::floor(currentEvent.mouseButton.x / 128) <= 7 &&
							std::floor(currentEvent.mouseButton.y / 96) >= 0 &&
							std::floor(currentEvent.mouseButton.y / 96 <= 7)) {
								ChessBoard::onClick(std::floor(currentEvent.mouseButton.x / 128),
													std::floor(currentEvent.mouseButton.y / 96));
							}
					}
				}
			}

			renderWindow.clear(sf::Color::Red);
			ChessBoard::draw(renderWindow);
			renderWindow.display();
		}

		// Spillet er nu slut
		// Loader billedet for hvem der vandt
		if (whitelost || blacklost) {
			renderWindow.clear(sf::Color::Red);

			sf::Sprite endscreen;
			if (whitelost) {
				endscreen.setTexture(Textures::endscreen_whitelost);
			}
			else if (blacklost) {
				endscreen.setTexture(Textures::endscreen_blacklost);
			}

			renderWindow.draw(endscreen);
			renderWindow.display();
		}

		// Venter på at spillerne trykker, så spillet starter igen
		while (gameover) {
			sf::Event currentEvent;
			while (renderWindow.pollEvent(currentEvent)) {
				if (currentEvent.type == sf::Event::Closed) {
					program_running = false;
					gameover = false;
					renderWindow.close();
				}
				if (currentEvent.type == sf::Event::KeyPressed) {
					if (currentEvent.key.code == sf::Keyboard::Escape) {
						program_running;
						gameover = false;
						renderWindow.close();
					}
					else {
						gameover = false;
						playing = true;
						ChessBoard::newGame();
					}
				}
				if (currentEvent.type == sf::Event::MouseButtonPressed) {
					gameover = false;
					playing = true;
					ChessBoard::newGame();
				}
			}
		}
	}
	
	// Programmet lukker
	return 0;
}