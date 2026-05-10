#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#ifdef _WIN32
#include <windows.h>   
#endif
using namespace std;


//  Enums:
enum class Color { WHITE, BLACK, NONE };
enum class PieceType { PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING, EMPTY };

//  Position struct:
struct Position
{
    int row, col;
    Position(int r, int c);
    bool isValid() const;
    bool operator==(const Position& o) const;
};

//  classes:
//  PosArray class:
class PosArray {
private:
    Position* data_;
    int size_;
    int cap_;
public:
    PosArray();
    PosArray(const PosArray& other);
    PosArray& operator=(const PosArray& other);
    ~PosArray();

    void push(const Position& p);
    int size() const;
    void clear();
    Position& operator[](int i);
    const Position& operator[](int i) const;
};

class Board;

//Piece class:
class Piece
{
protected:
    Color color_;
    PieceType type_;
    bool hasMoved_;
public:
    Piece(Color c, PieceType t);
    virtual ~Piece() {}

    Color getColor()    const;
    PieceType getType()     const;
    bool getHasMoved() const;
    void setHasMoved(bool v);
    virtual PosArray validMoves(const Position& from, const Board& board) const = 0;
    virtual char symbol()  const = 0;
};

//  Board Pieces classes:
class Pawn : public Piece
{
public:
    Pawn(Color c);
    PosArray validMoves(const Position& from, const Board& board) const;
    char symbol() const;
};

class Rook : public Piece
{
public:
    Rook(Color c);
    PosArray validMoves(const Position& from, const Board& board) const;
    char symbol() const;
};

class Knight : public Piece
{
public:
    Knight(Color c);
    PosArray validMoves(const Position& from, const Board& board) const;
    char symbol() const;
};

class Bishop : public Piece
{
public:
    Bishop(Color c);
    PosArray validMoves(const Position& from, const Board& board) const;
    char symbol() const;
};

class Queen : public Piece
{
public:
    Queen(Color c);
    PosArray validMoves(const Position& from, const Board& board) const;
    char symbol() const;
};

class King : public Piece
{
public:
    King(Color c);
    PosArray validMoves(const Position& from, const Board& board) const;
    char symbol() const;
};

//  Board  class:
class Board
{
private:
    Piece* grid_[8][8];
    void clearGrid();
    void placePieces();
public:
    Board();
    ~Board();
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;

    Piece* getPiece(const Position& p) const;
    Piece* getPiece(int r, int c) const;
    bool isEmpty(const Position& p) const;
    bool isEnemy(const Position& p, Color c) const;

    void movePiece(const Position& from, const Position& to);
    void setPiece(const Position& p, Piece* newPiece);

    // Template:
    template <typename Func>
    void simulateMove(const Position& from, const Position& to, Func fn)
    {
        Piece* moving = grid_[from.row][from.col];
        Piece* captured = grid_[to.row][to.col];
        grid_[to.row][to.col] = moving;
        grid_[from.row][from.col] = nullptr;
        fn();
        grid_[from.row][from.col] = moving;
        grid_[to.row][to.col] = captured;
    }

    PosArray legalMoves(const Position& from, Color c) const;
    bool     isInCheck(Color c) const;
    bool     hasAnyLegalMove(Color c) const;
    Position findKing(Color c) const;

    void reset();
    void display(const string& p1name, const string& p2name) const;
};

//  Game class:
class Game
{
private:
    Board board_;
    string player1_;   // White
    string player2_;   // Black
    Color currentTurn_;
    bool gameOver_;

    Color opponent(Color c)               const;
    Position parseSquare(const string& s, bool& ok) const;
    string posToStr(const Position& p)     const;
    void promotePawn(const Position& pos);
    void switchTurn();
public:
    Game();
    void setPlayers(const string& p1, const string& p2);
    void run();
};


//  Interface helpers:
struct TermSize
{
    int w;
    int h;
};   // terminal width / height in characters

TermSize getTerminalSize();
void vpad(int contentLines, int termHeight);  // prints blank lines to vertically centre
void clearScreen();
void showWelcome();
bool confirmExit();
void getPlayerNames(string& p1, string& p2);