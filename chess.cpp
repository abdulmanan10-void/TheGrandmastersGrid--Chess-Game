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

//  King
King::King(Color c) : Piece(c, PieceType::KING)
{
}
char King::symbol() const
{
    return (color_ == Color::WHITE) ? 'K' : 'k';
}
PosArray King::validMoves(const Position& from, const Board& board) const
{
    PosArray moves;
    for (int dr = -1; dr <= 1; ++dr)
    {
        for (int dc = -1; dc <= 1; ++dc)
        {
            if (dr == 0 && dc == 0) continue;
            Position p(from.row + dr, from.col + dc);
            if (p.isValid() && (board.isEmpty(p) || board.isEnemy(p, color_)))
                moves.push(p);
        }
    }
    return moves;
}

//  Board
Board::Board()
{
    clearGrid();
    placePieces();
}
Board::~Board()
{
    for (int r = 0; r < 8; ++r)
    {
        for (int c = 0; c < 8; ++c)
            delete grid_[r][c];
    }
}
void Board::clearGrid()
{
    for (int r = 0; r < 8; ++r)
    {
        for (int c = 0; c < 8; ++c)
            grid_[r][c] = nullptr;
    }
}
void Board::placePieces()
{
    // Black
    grid_[0][0] = new Rook(Color::BLACK);
    grid_[0][1] = new Knight(Color::BLACK);
    grid_[0][2] = new Bishop(Color::BLACK);
    grid_[0][3] = new Queen(Color::BLACK);
    grid_[0][4] = new King(Color::BLACK);
    grid_[0][5] = new Bishop(Color::BLACK);
    grid_[0][6] = new Knight(Color::BLACK);
    grid_[0][7] = new Rook(Color::BLACK);
    for (int c = 0; c < 8; ++c)
    {
        grid_[1][c] = new Pawn(Color::BLACK);
    }
    // White 
    grid_[7][0] = new Rook(Color::WHITE);
    grid_[7][1] = new Knight(Color::WHITE);
    grid_[7][2] = new Bishop(Color::WHITE);
    grid_[7][3] = new Queen(Color::WHITE);
    grid_[7][4] = new King(Color::WHITE);
    grid_[7][5] = new Bishop(Color::WHITE);
    grid_[7][6] = new Knight(Color::WHITE);
    grid_[7][7] = new Rook(Color::WHITE);
    for (int c = 0; c < 8; ++c)
    {
        grid_[6][c] = new Pawn(Color::WHITE);
    }
}
void Board::reset()
{
    for (int r = 0; r < 8; ++r)
    {
        for (int c = 0; c < 8; ++c)
        {
            delete grid_[r][c];
            grid_[r][c] = nullptr;
        }
    }
    placePieces();
}
Piece* Board::getPiece(const Position& p) const
{
    if (!p.isValid()) return nullptr;
    return grid_[p.row][p.col];
}
Piece* Board::getPiece(int r, int c) const
{
    return getPiece(Position(r, c));
}
bool Board::isEmpty(const Position& p) const
{
    return p.isValid() && !grid_[p.row][p.col];
}
bool Board::isEnemy(const Position& p, Color c) const
{
    if (!p.isValid()) return false;
    Piece* pc = grid_[p.row][p.col];
    return pc && pc->getColor() != c;
}
void Board::movePiece(const Position& from, const Position& to)
{
    delete grid_[to.row][to.col];
    grid_[to.row][to.col] = grid_[from.row][from.col];
    grid_[from.row][from.col] = nullptr;
    if (grid_[to.row][to.col]) grid_[to.row][to.col]->setHasMoved(true);
}
void Board::setPiece(const Position& p, Piece* newPiece)
{
    delete grid_[p.row][p.col];
    grid_[p.row][p.col] = newPiece;
}
Position Board::findKing(Color c) const
{
    for (int r = 0; r < 8; ++r)
    {
        for (int col = 0; col < 8; ++col)
        {
            if (grid_[r][col] && grid_[r][col]->getType() == PieceType::KING && grid_[r][col]->getColor() == c)
            {
                return Position(r, col);
            }
        }
    }
    return Position(-1, -1);
}
bool Board::isInCheck(Color c) const
{
    Position king = findKing(c);
    Color    enemy = (c == Color::WHITE) ? Color::BLACK : Color::WHITE;
    for (int r = 0; r < 8; ++r)
    {
        for (int col = 0; col < 8; ++col)
        {
            Piece* p = grid_[r][col];
            if (!p || p->getColor() != enemy) continue;
            PosArray mv = p->validMoves(Position(r, col), *this);
            for (int i = 0; i < mv.size(); ++i)
            {
                if (mv[i] == king) return true;
            }
        }
    }
    return false;
}
PosArray Board::legalMoves(const Position& from, Color c) const
{
    PosArray legal;
    Piece* p = getPiece(from);
    if (!p || p->getColor() != c) return legal;

    PosArray pseudo = p->validMoves(from, *this);
    Board& self = const_cast<Board&>(*this);

    for (int i = 0; i < pseudo.size(); ++i)
    {
        bool inCheck = false;
        self.simulateMove(from, pseudo[i], [&]() { inCheck = self.isInCheck(c); });
        if (!inCheck) legal.push(pseudo[i]);
    }
    return legal;
}
bool Board::hasAnyLegalMove(Color c) const
{
    for (int r = 0; r < 8; ++r)
    {
        for (int col = 0; col < 8; ++col)
        {
            Piece* p = grid_[r][col];
            if (!p || p->getColor() != c) continue;
            PosArray mv = legalMoves(Position(r, col), c);
            if (mv.size() > 0) return true;
        }
    }
    return false;
}

// Board display
void Board::display(const string& p1name, const string& p2name) const
{
    // ANSI 
    const char* RS = "\033[0m";
    const char* LIGHT_BG = "\033[48;5;241m";  // dark grey — light square
    const char* DARK_BG = "\033[48;5;232m";  // balck - dark square
    const char* WHITE_PC = "\033[1;97m";       // white pieces
    const char* BLACK_PC = "\033[38;5;55m";   // dark purple — black pieces
    const char* GOLD = "\033[0;33m";       // gold — row/col labels
    const char* BORDER = "\033[0;37m";       // grey — board border lines
    const char* DP = "\033[38;5;55m";   // dark purple — black legend box

    // Layout 
    const string BORDER_LINE = "    +-----+-----+-----+-----+-----+-----+-----+-----+";
    const string COL_HEADER = "       a     b     c     d     e     f     g     h";
    const string GAP_TO_LEG = "        ";

    const int DATA_WIDTH = 56;  // widest line = data rows
    // padding:
    const string BORDER_PAD = "   ";
    const string HEADER_PAD = "      ";

    // Board lines:
    const int LEG_START = 5;   // legend starts at this board line index
    const int LEG_LINES = 10;

    string wTitle = "White (" + p1name + ")";
    string bTitle = "Black (" + p2name + ")";

    // Center a string 
    auto centre20 = [](const string& s) -> string
        {
            int len = (int)s.size();
            if (len >= 20) return s.substr(0, 20);
            int lpad = (20 - len) / 2;
            int rpad = 20 - len - lpad;
            return string(lpad, ' ') + s + string(rpad, ' ');
        };
    // Left-pad a string
    auto leftPad20 = [](const string& s) -> string
        {
            string entry = "  " + s;
            while ((int)entry.size() < 20) entry += ' ';
            return entry;
        };

    const char* PIECES[6][2] = {
        {"K=King",  "k=King"},  {"Q=Queen", "q=Queen"},
        {"R=Rook",  "r=Rook"},  {"B=Bishop","b=Bishop"},
        {"N=Knight","n=Knight"},{"P=Pawn",  "p=Pawn"}
    };
    const string BB = "+--------------------+";  // box border 
    const string BG = "     ";                   // gap between boxes 

    auto printLegendLine = [&](int li) {
        if (li == 0 || li == 2 || li == 9) {
            cout << BB << BG << DP << BB << RS;
        }
        else if (li == 1) {
            cout << "|" << centre20(wTitle) << "|"
                << BG
                << DP << "|" << centre20(bTitle) << "|" << RS;
        }
        else {
            int pi = li - 3;  // piece index 0..5
            cout << "|" << leftPad20(PIECES[pi][0]) << "|"
                << BG
                << DP << "|" << leftPad20(PIECES[pi][1]) << "|" << RS;
        }
        };

    // Print everything
    cout << "\n";

    for (int line = 0; line < 20; ++line)
    {

        if (line == 0 || line == 18) {
            // Column header / footer 
            cout << GOLD << COL_HEADER << RS << HEADER_PAD;

        }
        else if (line == 19) {
            // Final blank line
            cout << string(DATA_WIDTH, ' ');

        }
        else if (line % 2 == 1) {
            // Odd lines = border rows
            cout << BORDER << BORDER_LINE << RS << BORDER_PAD;
        }
        else {
            // Board rows (row 8 down to row 1)
            int r = (line / 2) - 1;   // r=0 when line=2, r=7 when line=16

            // Left row number:
            cout << GOLD << " " << (8 - r) << "  " << RS;
            cout << BORDER << "|" << RS;

            for (int c = 0; c < 8; ++c)
            {
                bool        light = (r + c) % 2 == 0;
                const char* bg = light ? LIGHT_BG : DARK_BG;
                Piece* p = grid_[r][c];

                if (p)
                {
                    const char* fg = (p->getColor() == Color::WHITE) ? WHITE_PC : BLACK_PC;

                    cout << bg << fg << "  " << p->symbol() << "  " << RS;
                }
                else {
                    cout << bg << "     " << RS;
                }
                cout << BORDER << "|" << RS;
            }

            // Right row number:
            cout << GOLD << "  " << (8 - r) << RS;
        }

        // Append line
        int li = line - LEG_START;
        if (li >= 0 && li < LEG_LINES)
        {
            cout << GAP_TO_LEG;
            printLegendLine(li);
        }

        cout << "\n";
    }
    cout << "\n";
}

//  Game:
Game::Game() : currentTurn_(Color::WHITE), gameOver_(false)
{
}
void Game::setPlayers(const string& p1, const string& p2)
{
    player1_ = p1;
    player2_ = p2;
}
Color Game::opponent(Color c) const
{
    return (c == Color::WHITE) ? Color::BLACK : Color::WHITE;
}
Position Game::parseSquare(const string& s, bool& ok) const
{
    ok = false;
    if (s.size() < 2) return {};
    char colCh = s[0];
    if (colCh >= 'A' && colCh <= 'Z') colCh = colCh + 32;
    char rowCh = s[1];
    if (colCh < 'a' || colCh > 'h') return {};
    if (rowCh < '1' || rowCh > '8') return {};
    ok = true;
    return Position(8 - (rowCh - '0'), colCh - 'a');
}
string Game::posToStr(const Position& p) const
{
    string s = "  ";
    s[0] = (char)('a' + p.col);
    s[1] = (char)('0' + (8 - p.row));
    return s;
}
void Game::switchTurn()
{
    currentTurn_ = opponent(currentTurn_);
}
void Game::promotePawn(const Position& pos)
{
    Piece* p = board_.getPiece(pos);
    if (!p || p->getType() != PieceType::PAWN) return;
    bool isPromRow = (p->getColor() == Color::WHITE && pos.row == 0) || (p->getColor() == Color::BLACK && pos.row == 7);
    if (!isPromRow) return;

    Color  c = p->getColor();
    string name = (c == Color::WHITE) ? player1_ : player2_;
    char   ch = 'Q';

    const char* GOLD = "\033[0;33m";
    const char* RS = "\033[0m";

    cout << GOLD << "\n  *** PAWN PROMOTION! ***" << RS << "\n";
    cout << GOLD << "  Choose: Q=Queen  R=Rook  B=Bishop  N=Knight" << RS << "\n";
    cout << GOLD << "  Enter choice: ";
    cin >> ch;
    cout << RS;
    if (ch >= 'a' && ch <= 'z') ch = ch - 32;

    Piece* newPiece = nullptr;
    switch (ch) {
    case 'R': newPiece = new Rook(c);   break;
    case 'B': newPiece = new Bishop(c); break;
    case 'N': newPiece = new Knight(c); break;
    default:  newPiece = new Queen(c);  break;
    }
    board_.setPiece(pos, newPiece);
    cout << GOLD << "  Promoted to " << newPiece->symbol() << "!" << RS << "\n";
}

// Centered message box 
static void printMsgBox(const string& msg, const char* color, int termW) {
    const char* RS = "\033[0m";
    int inner = (int)msg.size() + 4;
    string top = "+" + string(inner, '-') + "+";
    string middle = "|  " + msg + "  |";
    string bot = "+" + string(inner, '-') + "+";

    int pad = (termW - (int)top.size()) / 2;
    if (pad < 0) pad = 0;
    string indent(pad, ' ');

    cout << color << indent << top << RS << "\n";
    cout << color << indent << middle << RS << "\n";
    cout << color << indent << bot << RS << "\n";
}