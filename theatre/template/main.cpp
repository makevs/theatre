#include <iostream>
#include <vector>

using namespace std;

// Fields in the input file
const int NUMBER_OF_FIELDS = 5;

// Command prompt
const string PROMPT = "the> ";

// Error and other messages
const string EMPTY_FIELD = "Error: empty field in line ";
const string FILE_ERROR = "Error: input file cannot be opened";
const string WRONG_PARAMETERS = "Error: wrong number of parameters";
const string THEATRE_NOT_FOUND = "Error: unknown theatre";
const string PLAY_NOT_FOUND = "Error: unknown play";
const string PLAYER_NOT_FOUND = "Error: unknown player";
const string TOWN_NOT_FOUND = "Error: unknown town";
const string COMMAND_NOT_FOUND = "Error: unknown command";
const string NOT_AVAILABLE = "No plays available";

// Casual split function, if delim char is between "'s, ignores it.
vector<string> split(const string& str, char delim)
{
    vector<string> result = {""};
    bool inside_quotation = false;
    for(char current_char : str)
    {
        if(current_char == '"')
        {
            inside_quotation = not inside_quotation;
        }
        else if(current_char == delim and not inside_quotation)
        {
            result.push_back("");
        }
        else
        {
            result.back().push_back(current_char);
        }
    }
    return result;
}

// Main function
int main()
{
    return EXIT_SUCCESS;
}
