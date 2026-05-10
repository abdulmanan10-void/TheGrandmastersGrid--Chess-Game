#include"chess.h"

//  Position struct:
Position::Position(int r = -1, int c = -1) : row(r), col(c)
{
}
bool Position::isValid() const
{
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}
bool Position::operator==(const Position& o) const
{
    return row == o.row && col == o.col;
}

//  PosArray class:
PosArray::PosArray() : data_(nullptr), size_(0), cap_(0)
{
}
PosArray::PosArray(const PosArray& o) : data_(nullptr), size_(0), cap_(0)
{
    for (int i = 0; i < o.size_; ++i) push(o.data_[i]);
}
PosArray& PosArray::operator=(const PosArray& o)
{
    if (this == &o)
    {
        return *this;
    }
    delete[] data_;
    data_ = nullptr; size_ = 0; cap_ = 0;
    for (int i = 0; i < o.size_; ++i)
    {
        push(o.data_[i]);
    }
    return *this;
}
PosArray::~PosArray()
{
    delete[] data_;
}
void PosArray::push(const Position& p)
{
    if (size_ == cap_)
    {
        int newCap = (cap_ == 0) ? 4 : cap_ * 2;
        Position* tmp = new Position[newCap];
        for (int i = 0; i < size_; ++i)
        {
            tmp[i] = data_[i];
        }
        delete[] data_;
        data_ = tmp;
        cap_ = newCap;
    }
    data_[size_++] = p;
}
int PosArray::size() const
{
    return size_;
}
void PosArray::clear()
{
    size_ = 0;
}
Position& PosArray::operator[](int i)
{
    return data_[i];
}
const Position& PosArray::operator[](int i) const
{
    return data_[i];
}

//Piece class:
Piece::Piece(Color c, PieceType t) : color_(c), type_(t), hasMoved_(false)
{
}
Color Piece::getColor() const
{
    return color_;
}
PieceType Piece::getType() const
{
    return type_;
}
bool Piece::getHasMoved() const
{
    return hasMoved_;
}
void Piece::setHasMoved(bool v)
{
    hasMoved_ = v;
}

//  Board Pieces classes:
//  Pawn
Pawn::Pawn(Color c) : Piece(c, PieceType::PAWN)
{
}
char Pawn::symbol() const
{
    return (color_ == Color::WHITE) ? 'P' : 'p';
}
PosArray Pawn::validMoves(const Position& from, const Board& board) const
{
    PosArray moves;
    int dir = (color_ == Color::WHITE) ? -1 : 1;

    Position one(from.row + dir, from.col);
    if (one.isValid() && board.isEmpty(one))
    {
        moves.push(one);
        bool startRank = (color_ == Color::WHITE) ? (from.row == 6) : (from.row == 1);
        Position two(from.row + 2 * dir, from.col);
        if (startRank && two.isValid() && board.isEmpty(two))
        {
            moves.push(two);
        }
    }
    int dcs[2] = { -1, 1 };
    for (int i = 0; i < 2; ++i) {
        Position diag(from.row + dir, from.col + dcs[i]);
        if (diag.isValid() && board.isEnemy(diag, color_))
            moves.push(diag);
    }
    return moves;
}

//  Rook
Rook::Rook(Color c) : Piece(c, PieceType::ROOK)
{
}
char Rook::symbol() const
{
    return (color_ == Color::WHITE) ? 'R' : 'r';
}
PosArray Rook::validMoves(const Position& from, const Board& board) const
{
    PosArray moves;
    int dirs[4][2] = { {-1,0},{1,0},{0,-1},{0,1} };
    for (int d = 0; d < 4; ++d)
    {
        Position cur(from.row + dirs[d][0], from.col + dirs[d][1]);
        while (cur.isValid())
        {
            if (board.isEmpty(cur))
            {
                moves.push(cur);
            }
            else
            {
                if (board.isEnemy(cur, color_))
                {
                    moves.push(cur);
                    break;
                }
            }
            cur.row += dirs[d][0]; cur.col += dirs[d][1];
        }
    }
    return moves;
}

//  Knight
Knight::Knight(Color c) : Piece(c, PieceType::KNIGHT)
{
}
char Knight::symbol() const
{
    return (color_ == Color::WHITE) ? 'N' : 'n';
}
PosArray Knight::validMoves(const Position& from, const Board& board) const
{
    PosArray moves;
    int jumps[8][2] = { {-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1} };
    for (int i = 0; i < 8; ++i)
    {
        Position p(from.row + jumps[i][0], from.col + jumps[i][1]);
        if (p.isValid() && (board.isEmpty(p) || board.isEnemy(p, color_)))
        {
            moves.push(p);
        }
    }
    return moves;
}

//  Bishop
Bishop::Bishop(Color c) : Piece(c, PieceType::BISHOP)
{
}
char Bishop::symbol() const
{
    return (color_ == Color::WHITE) ? 'B' : 'b';
}
PosArray Bishop::validMoves(const Position& from, const Board& board) const
{
    PosArray moves;
    int dirs[4][2] = { {-1,-1},{-1,1},{1,-1},{1,1} };
    for (int d = 0; d < 4; ++d) {
        Position cur(from.row + dirs[d][0], from.col + dirs[d][1]);
        while (cur.isValid())
        {
            if (board.isEmpty(cur))
            {
                moves.push(cur);
            }
            else
            {
                if (board.isEnemy(cur, color_))
                {
                    moves.push(cur);
                    break;
                }
            }
            cur.row += dirs[d][0]; cur.col += dirs[d][1];
        }
    }
    return moves;
}

//  Queen
Queen::Queen(Color c) : Piece(c, PieceType::QUEEN)
{
}
char Queen::symbol()  const
{
    return (color_ == Color::WHITE) ? 'Q' : 'q';
}
PosArray Queen::validMoves(const Position& from, const Board& board) const
{
    PosArray moves;
    int dirs[8][2] = { {-1,0},{1,0},{0,-1},{0,1},{-1,-1},{-1,1},{1,-1},{1,1} };
    for (int d = 0; d < 8; ++d)
    {
        Position cur(from.row + dirs[d][0], from.col + dirs[d][1]);
        while (cur.isValid())
        {
            if (board.isEmpty(cur))
            {
                moves.push(cur);
            }
            else
            {
                if (board.isEnemy(cur, color_))
                {
                    moves.push(cur);
                    break;
                }
            }
            cur.row += dirs[d][0]; cur.col += dirs[d][1];
        }
    }
    return moves;
}

