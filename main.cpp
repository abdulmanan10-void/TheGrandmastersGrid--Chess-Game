#include "chess.h"

int main()
{
    string p1, p2;

    while (true)
    {
        showWelcome();

        const char* O = "\033[38;5;214m";
        cout << O;
        char choice;
        cin >> choice;

        if (choice == '2')
        {
            if (confirmExit())
            {
                clearScreen();
                const char* RS = "\033[0m";
                TermSize ts = getTerminalSize();
                const int MSG_W = 54;
                vpad(1, ts.h);
                cout << O;
                cout << string((ts.w - MSG_W) / 2, ' ')
                    << "Thank you for playing The Grandmaster's Grid. Goodbye!" << RS << "\n\n";
                system("pause");
                return 0;
            }
        }
        else if (choice == '1')
        {
            getPlayerNames(p1, p2);

            Game game;
            game.setPlayers(p1, p2);
            game.run();
        }
        else
        {
            clearScreen();
            cout << "  Invalid option. Please enter 1 or 2." << endl;
            system("pause");
        }
    }

    return 0;
}