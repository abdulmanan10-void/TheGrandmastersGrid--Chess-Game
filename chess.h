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
