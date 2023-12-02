/* Project 2: Theatre
 *
 * Program author
 * Name: Marko **************
 * Student number: *****************
 * UserID: **********
 * E-Mail: m******************@tuni.fi
 *
 * A program that reads data from a file given by user, and stores it in
 * memory using two different maps. The program has a set of features to look up
 * information from the data structure, and prints it to the user.
 *
 * The std namespace has been omitted again explicitly on purpose
 * due to personal preferences.
 *
 * */

#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>

// Fields in the input file
const int NUMBER_OF_FIELDS = 5;

// Command prompt
const std::string PROMPT = "the> ";

// Error and other messages
const std::string EMPTY_FIELD = "Error: empty field in line ";
const std::string FILE_ERROR = "Error: input file cannot be opened";
const std::string WRONG_PARAMETERS = "Error: wrong number of parameters";
const std::string THEATRE_NOT_FOUND = "Error: unknown theatre";
const std::string PLAY_NOT_FOUND = "Error: unknown play";
const std::string PLAYER_NOT_FOUND = "Error: unknown player";
const std::string TOWN_NOT_FOUND = "Error: unknown town";
const std::string COMMAND_NOT_FOUND = "Error: unknown command";
const std::string NOT_AVAILABLE = "No plays available";

// Commands with integer values for use of switch expression in main function
const std::map<std::string, int> COMMANDS = {
        {"quit", 1},
        {"theatres", 2},
        {"plays", 3},
        {"theatres_of_play", 4},
        {"plays_in_theatre", 5},
        {"plays_in_town", 6},
        {"players_in_play", 7}
};


// Casual split function, if delim char is between "'s, ignores it.
std::vector<std::string> split(const std::string& str, char delim)
{
    std::vector<std::string> result = {""};
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


// A simple container storing necessary data about plays in each theatre.
struct theatres {
    std::string play_name;
    std::string player_name;
    int free_seats;
};


bool lookup_play(const std::map<std::string,
                    std::vector<theatres>>& theatre_data,
                 const std::string& play_name)
{
    /* A reusable function to check if a play exists in the data structure.
     * Returns true if found, false if not. */

    for (const std::pair<const std::string,
            std::vector<theatres>>& t_name: theatre_data)
    {
        for (const theatres& t_plays : t_name.second) {
            std::vector<std::string> split_name = split(t_plays.play_name, '/');

            for (const std::string& name : split_name) {
                if (name == play_name) {
                    return true;
                }
            }
        }
    }
    std::cout << PLAY_NOT_FOUND << std::endl;
    return false;
}


bool lookup_theatre(const std::map<std::string,
                        std::vector<theatres>>& theatre_data,
                    const std::string& theatre_name)
{
    /* A reusable function to check if a theatre exists in the data structure.
     * Returns true if found, false if not. */

    if (theatre_data.find(theatre_name) == theatre_data.end()) {
        std::cout << THEATRE_NOT_FOUND << std::endl;
        return false;
    } else {
        return true;
    }
}


void read_file(std::map<std::string, std::vector<std::string>>& town_data,
               std::map<std::string, std::vector<theatres>>& theatre_data)
{
    /* Gets the file input from user, reads data from it if applicable,
     * else prints an error message and the program will exit due to
     * conditional check on the container size. */

    std::cout << "Input file: ";
    std::string file_name;
    std::getline(std::cin, file_name);
    std::ifstream file_object(file_name);

    if (not file_object) {
        std::cout << FILE_ERROR << std::endl;
        return;
    } else {
        std::string line;
        int line_number = 1;
        int seats = 0;

        while (getline(file_object, line)) {
            std::vector<std::string> split_line = split(line, ';');

            if (split_line.size() != NUMBER_OF_FIELDS) {
                std::cout << EMPTY_FIELD << line_number << std::endl;

                // Satisfy exit condition on main function
                town_data.clear();
                return;

            } else {
                // If the seats field is "none", it is converted to 0
                if (split_line.at(4)== "none") {
                    seats = 0;
                } else {
                    // A try-catch could be useful here.
                    seats = stoi(split_line.at(4));
                }

                std::string temp_city = split_line.at(0);
                std::string temp_theatre = split_line.at(1);
                std::string temp_play = split_line.at(2);
                std::string temp_player = split_line.at(3);
                theatres temp_plays = {temp_play,
                                 temp_player,
                                 seats};

                if (theatre_data.find(temp_theatre) == theatre_data.end()) {
                    town_data[temp_city].push_back(temp_theatre);
                }
                theatre_data[temp_theatre].push_back(temp_plays);
            }
            line_number++;
        }
    }
}


// Simple function to print all the theatres in alphabetical order
void print_theatres(const std::map<std::string,
                    std::vector<theatres>>& theatre_data)
{
    for (const std::pair<std::string,
            std::vector<theatres>>& t_name:
            theatre_data)
    {
        std::cout << t_name.first << std::endl;
    }
}

// String comparison for use with sorting algorithm
bool sort_alphabetically(const std::string& a,const std::string& b) {
    return a < b;
}

void plays(const std::map<std::string, std::vector<theatres>>& theatre_data) {
    /* Prints all the plays in alphabetical order, if the play name
     * has an alternative name in English, it is reformatted.
     * Temporary container is a bit redundant here, since the program doesn't
     * actually allow adding new plays during runtime, but I just wanted to use
     * less data structures in the main scope and keep the initial file
     * reading function a bit less complex.
     * */

    std::vector<std::string> temp_plays;

    for (const std::pair<std::string,
            std::vector<theatres>>& t_name:
            theatre_data)
    {
        for (const theatres& t_plays: t_name.second) {
            // Check for duplicates
            if (std::find(temp_plays.begin(),
                          temp_plays.end(),
                          t_plays.play_name) == temp_plays.end()) {
                temp_plays.push_back(t_plays.play_name);
            }
        }
    }

    // Sort the container in alphabetical order.
    std::sort(temp_plays.begin(),
              temp_plays.end(),
              sort_alphabetically);

    for (const std::string& play_name: temp_plays) {
        // Conditional check for bilingual formatting
        if (split(play_name, '/').size() > 1) {
            std::vector<std::string> temp = split(play_name, '/');
            std::cout << temp.at(0) << " *** " << temp.at(1) << std::endl;
        } else {
            std::cout << play_name << std::endl;
        }
    }
}

// Prints all the theatres that have the play given as command parameter
void theatres_of_play(const std::map<std::string,
                            std::vector<theatres>>& theatre_data,
                      const std::string& play_name)
{
    if (not lookup_play(theatre_data, play_name)) {
        return;
    }

    std::vector<std::string> temp_theatres;

    for (const std::pair<std::string,
            std::vector<theatres>>& t_name: theatre_data)
    {
        for (const theatres& t_plays: t_name.second) {
            // Check for duplicates
            std::vector<std::string> split_name = split(t_plays.play_name, '/');

            if (std::find(split_name.begin(),
                          split_name.end(),
                          play_name) != split_name.end()) {
                if (std::find(temp_theatres.begin(),
                              temp_theatres.end(),
                              t_name.first) == temp_theatres.end()) {
                    temp_theatres.push_back(t_name.first);
                }
            }
        }
    }

    // Alphabetical sorting of the temporary container
    std::sort(temp_theatres.begin(),
              temp_theatres.end(),
              sort_alphabetically);

    for (const std::string& t_name: temp_theatres) {
        std::cout << t_name << std::endl;
    }
}


// Prints all the plays available in a theatre
void plays_in_theatre(const std::map<std::string,
                        std::vector<theatres>>& theatre_data,
                      const std::string& theatre_name)
{
    // Check first if the theatre given exists
    if (not lookup_theatre(theatre_data, theatre_name)) {
        return;
    }

    for (const std::pair<std::string,
            std::vector<theatres>>& t_name: theatre_data)
    {
        if (t_name.first == theatre_name) {
            std::vector<std::string> temp_plays;
            for (const theatres& t_plays: t_name.second) {
                // Check for duplicates
                if (std::find(temp_plays.begin(),
                              temp_plays.end(),
                              t_plays.play_name) == temp_plays.end()) {
                    temp_plays.push_back(t_plays.play_name);
                }
            }

            // Sort the container in alphabetical order.
            std::sort(temp_plays.begin(),
                      temp_plays.end(),
                      sort_alphabetically);

            for (const std::string& play: temp_plays) {
                // Conditional check for bilingual formatting
                if (split(play, '/').size() > 1) {
                    std::vector<std::string> temp = split(play, '/');
                    std::cout << temp.at(0)
                        << " *** "
                        << temp.at(1) << std::endl;
                } else {
                    std::cout << play << std::endl;
                }
            }
        }
    }
}


void plays_in_town(const std::map<std::string,
                        std::vector<std::string>>& town_data,
                   const std::map<std::string,
                        std::vector<theatres>>& theatre_data,
                   const std::string& town_name)
{
    // Check first if the town given exists
    if (town_data.find(town_name) == town_data.end()) {
        std::cout << TOWN_NOT_FOUND << std::endl;
        return;
    }

    int valid_plays = 0;

    for (const std::string& theatre: town_data.at(town_name)) {
        std::map<std::string, int> temp_plays;

        // Create a temporary map of plays and free seats in a theatre
        for(const theatres& t_plays: theatre_data.at(theatre)) {
            temp_plays[t_plays.play_name] = t_plays.free_seats;
        }
        // Iterating the map and printing out the plays with free seats
        for(const std::pair<std::string, int>& play_info: temp_plays) {
            // Check for free seats
            if(play_info.second > 0) {
                std::vector<std::string> split_play = split(play_info.first,
                                                            '/');
                // Check for plays with English name
                if (split_play.size() == 2) {
                    std::cout << theatre << " : " << split_play.at(0)
                    << " --- " << split_play.at(1) << " : "
                    << play_info.second << std::endl;
                } else {
                    std::cout << theatre << " : " << play_info.first << " : "
                    << play_info.second << std::endl;
                }
                valid_plays++;
            }
        }
    }
    // Error message printed out if there were no plays with free seats in town.
    if (valid_plays == 0) {
        std::cout << NOT_AVAILABLE << std::endl;
    }
}


void players_in_play(const std::map<std::string,
                        std::vector<theatres>>& theatre_data,
                     const std::string& play_name,
                     const std::string& theatre_name)
{
    /* Find info about players in the play, empty input for theatre name is
     * supplised by default if the parameter is left empty. The function first
     * checks validity of user input, with theatre name first if given. */

    if (theatre_name != "" and not lookup_theatre(theatre_data, theatre_name)) {
        return;
    }

    if (not lookup_play(theatre_data, play_name)) {
        return;
    }

    for (const std::pair<std::string,
            std::vector<theatres>>& theatre: theatre_data)
    {
        // Factoring in the possible empty input for theatre name
        if (theatre.first == theatre_name or theatre_name == "") {
            std::vector<std::string> temp_players;
            for (const theatres& t_play : theatre.second) {
                std::vector<std::string> split_name = split(t_play.play_name,
                                                            '/');
                for (const std::string& p_name: split_name) {
                    if (p_name == play_name) {
                        temp_players.push_back(t_play.player_name);
                    }
                }
            }
            std::sort(temp_players.begin(),
                      temp_players.end(),
                      sort_alphabetically);

            for (const std::string& player: temp_players) {
                std::cout << theatre.first << " : " << player <<std::endl;
            }
        }
    }
}

// Main function
int main()
{
    // A map representing the towns as key and vector of theatres as value
    std::map<std::string, std::vector<std::string>> town_data;

    // A map of theatres with a vector of theatres struct as value
    std::map<std::string, std::vector<theatres>> theatre_data;

    // Reading the file and storing the data into structures
    read_file(town_data, theatre_data);

    if (town_data.size()==0) {
        return EXIT_FAILURE;
    } else {
        // Do stuff

        while (true) {
            std::cout << PROMPT;
            std::string command = "";
            std::string command_parameter = "";
            std::string command_parameter2 = "";
            std::getline(std::cin, command);

            std::vector<std::string> split_command = split(command, ' ');
            command = split_command.at(0);
            if (split_command.size() >= 2) {
                command_parameter = split_command.at(1);
            }
            if (split_command.size() == 3) {
                command_parameter2 = split_command.at(2);
            }

            if (COMMANDS.find(command) == COMMANDS.end()){
                std::cout << COMMAND_NOT_FOUND << std::endl;
                continue;
            }
            switch (COMMANDS.at(command)) {
                // Quit
                case 1:
                    return EXIT_SUCCESS;

                // Theatres
                case 2:
                    print_theatres(theatre_data);
                    break;

                // Plays
                case 3:
                    if (command_parameter != "") {
                        std::cout << WRONG_PARAMETERS << std::endl;
                        continue;
                    }
                    plays(theatre_data);
                    break;

                // Theatres of play
                case 4:
                    theatres_of_play(theatre_data,command_parameter);
                    break;

                // Plays in theatre
                case 5:
                    plays_in_theatre(theatre_data, command_parameter);
                    break;

                // Plays in town
                case 6:
                    plays_in_town(town_data, theatre_data, command_parameter);
                    break;

                // Players in play
                case 7:
                    players_in_play(theatre_data, command_parameter,
                                    command_parameter2);
                    break;
            }
        }
    }
}
