#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <cstdlib> // for rand and srand
#include <ctime>   // for time
#include <string>

using json = nlohmann::json;

void clear_screen()
{
#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Unix/Linux
#endif
}

// Function to replace HTML entities with their corresponding characters
std::string decodeHtmlEntities(const std::string &input)
{
    std::string result = input;

    // Replace &quot; with "
    size_t pos = result.find("&quot;");
    while (pos != std::string::npos)
    {
        result.replace(pos, 6, "\"");
        pos = result.find("&quot;", pos + 1);
    }

    // Replace &#039; with '
    pos = result.find("&#039;");
    while (pos != std::string::npos)
    {
        result.replace(pos, 6, "\'");
        pos = result.find("&quot;", pos + 1);
    }

    // You can add more replacements for other HTML entities as needed

    return result;
}

int letterToNumber(char letter)
{
    if (std::isupper(letter))
    {
        return letter - 'A' + 1;
    }
    else if (std::islower(letter))
    {
        return letter - 'a' + 1;
    }
    else
    {
        // Handle non-letter characters, or you can return an error code
        return -1;
    }
}

char numberToLetter(int number)
{
    if (number >= 1 && number <= 26)
    {
        return static_cast<char>('a' + number - 1);
    }
    else
    {
        // Handle out-of-range numbers or return an error character
        return '?';
    }
}

void run_exam(int category_id, std::string category_name)
{
    std::cout << "Questions on " << category_name << "\n"
              << std::endl;
    std::cout << "********************************\n"
              << std::endl;

    cpr::Response response = cpr::Get(cpr::Url{"https://opentdb.com/api.php?amount=10&category=" + std::to_string(category_id)}, cpr::Header{{"accept", "application/json"}});

    // Check if the request was successful
    if (response.status_code == 200)
    {
        json data = json::parse(response.text);
        int count = 1;
        int score = 0;
        for (json question : data["results"])
        {
            // Decode HTML entities
            std::string decodedQuestion = decodeHtmlEntities(question["question"].get<std::string>());
            std::cout << "Question " << count++ << ". " << decodedQuestion << std::endl;

            // Show options
            json options = question["incorrect_answers"];
            // Get a random index within the vector's size
            int randomIndex = std::rand() % (options.size());
            // std::cout << "Random idx: " << randomIndex << ", size: " << options.size() << std::endl;
            std::string temp;
            int correct_idx;
            for (int i = 0; i < options.size(); i++)
            {
                if (i == randomIndex)
                {
                    temp = options[randomIndex].get<std::string>();
                    std::cout << numberToLetter(i + 1) << ".) " << question["correct_answer"].get<std::string>() << std::endl;
                    correct_idx = i + 1;
                    continue;
                }
                std::cout << numberToLetter(i + 1) << ".) " << options[i].get<std::string>() << std::endl;
            }
            std::cout << numberToLetter(options.size() + 1) << ".) " << temp << std::endl;
            std::cout << "Choice: ";
            char answer;
            std::cin >> answer;

            if (letterToNumber(answer) == correct_idx)
            {
                std::cout << "Correct. Answer: " << question["correct_answer"].get<std::string>() << std::endl;
                score++;
            }
            else
            {
                std::cout << "Incorrect. Answer: " << question["correct_answer"].get<std::string>() << std::endl;
            }
            std::cout << std::endl;
        }
        std::cout << "Final Score: " << score << std::endl;
    }
    else
    {
        std::cerr << "Request failed with status code: " << response.status_code << std::endl;
    }
}

int main()
{

    // clear screen for application
    clear_screen();

    // Make a GET request to a URL
    cpr::Response response = cpr::Get(cpr::Url{"https://opentdb.com/api_category.php"}, cpr::Header{{"accept", "application/json"}});

    // Check if the request was successful
    if (response.status_code == 200)
    {
        std::cout << "QUESTIONAIRE APP\n"
                  << std::endl;
        std::cout << "********************************\n"
                  << std::endl;

        json data = json::parse(response.text);
        int count = 1;
        for (json d : data["trivia_categories"])
        {
            std::cout << count++ << ". " << d["name"].get<std::string>() << std::endl;
        }

        std::cout << "********************************\n"
                  << std::endl;
        std::cout << "Select a category for questions: ";

        int category_no;
        std::cin >> category_no;

        // std::cout << "Count: " << data["trivia_categories"].size() << std::endl;
        int category_length = data["trivia_categories"].size();
        if (category_no <= 0 || category_no > category_length)
        {
            std::cout << "You selected an invalid category" << std::endl;
        }
        else
        {
            // get the questions
            count = 1;
            for (json j : data["trivia_categories"])
            {
                if (category_no == count)
                {
                    // clear screen for questions
                    clear_screen();
                    run_exam(j["id"].get<int>(), j["name"].get<std::string>());
                    break;
                }
                count++;
            }
        }
    }
    else
    {
        std::cerr << "Request failed with status code: " << response.status_code << std::endl;
    }

    return 0;
}
