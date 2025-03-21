#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>

std::map<char, int> word_to_map(const std::string& word) {
    std::map<char, int> out;
    for(const auto& l : word) {
        out[l] = out[l] + 1;
    }
    return out;
}

std::string attempt_to_key (const std::string& attempt, const std::string& solution) {
    std::map<char, int> solution_map = word_to_map(solution);
    std::map<char, int> attempt_map = word_to_map(attempt);
    std::stringstream out;
    for(int i = 0; i < solution.length(); i++) {
        char l = attempt[i];
        if (solution_map[l] == 0 || (attempt_map[l] > solution_map[l])) {
            out << "#";
        } else {
            if (solution[i] == l) {
                out << "=";
            } else {
                out << "+";
            }
            solution_map[l] = solution_map[l] - 1;
        }
    }

    return out.str();
}

bool find_letter(const std::string& domain, char l) {
    for (auto l_domain : domain) {
        if (l == l_domain) {
            return 1;
        }
    }

    return 0;
}

void remove_letter (std::string& domain, const char& l) {
    for (auto it = domain.begin(); it != domain.end(); ++it) {
        if (*it == l) {
            *it = '#';
            return;
        }
    }
}

void solve_domain(std::string& domain, const char& l) {
    for(auto it = domain.begin(); it != domain.end(); ++it) {
        if (*it != l) {
            *it = '#';
        }
    }
}

std::string char_to_str(char c) {
    std::string out;
    out = c;
    return out;
}

std::string domains_to_str(const std::vector<std::string>& domains) {
    std::stringstream out;
    for(const auto& domain : domains){
        out << domain << "\n";
    }
    return out.str();
}
 
std::string must_be_present_to_str(const std::map<char, int>& mbp) {
    std::stringstream out;
    out << "{ ";
    for(const auto& mbp_pair : mbp){
        out << "<" << mbp_pair.first << ": " << mbp_pair.second << "> ";
    }
    out << "}";
    out << "\n";
    return out.str();
}

class WordleAssistant {
public:

    std::string solution;

    WordleAssistant(int n) : word_length(n) {
        for (int i = 0; i < word_length; i++) {
            domains.push_back("abcdefghijklmnopqrstuvwxyz");
            solution.push_back('#');
        }
    }

    std::vector<std::string> get_domains() const {
        return domains;
    }

    std::map<char, int> get_must_be_present() const {
        return must_be_present;
    }

    void load_attempt(std::pair<std::string, std::string> attempt) {
        std::map<char, int> attempt_map = word_to_map(attempt.first);
        std::map<char, int> attempt_correct;
        for (int i = 0; i < word_length; i++) {
            char l = attempt.first[i];
            char l_status = attempt.second[i];

            if (l_status == '+' || l_status == '=') {
                attempt_correct[l]++;
                if (attempt_correct[l] > must_be_present[l]) {
                    must_be_present[l] = attempt_correct[l];
                }
            }

            if (l_status =='+') {
                remove_letter(domains[i], l);
            }

            if (l_status == '=') {
                if (solution[i] != l) {
                    solve_domain(domains[i], l);
                    solution[i] = l;
                } 
            }
        }

        for(int i = 0; i < word_length; i++) {
            char l = attempt.first[i];
            char l_status = attempt.second[i];

            if(l_status = '#') {
                if(must_be_present.find(l) == must_be_present.end()) {
                    for (int j = 0; j < domains.size(); j++) {
                        if (l != solution[j]) {
                            remove_letter(domains[j], l);
                        }
                    }
                } else {
                    remove_letter(domains[i], l);

                }

            }
        }
    }

    bool compatible_with_attempts(std::string attempt) const {
        std::map<char, int> attempt_map;
        for (int i = 0; i < word_length; i++) {
            char l = attempt[i];
            attempt_map[l] = attempt_map[l] + 1;
            if (solution[i] != '#' && solution[i] != l) {
                return 0;
            }

            if (!find_letter(domains[i], l)) {
                return 0;
            }
        }

        for (auto l : must_be_present) {
            if (attempt_map[l.first] < l.second) {
                return 0;
            }
        }

        return 1;
    }



private:

    int word_length;
    std::vector<std::string> domains;
    std::map<char, int> must_be_present;

};

void test_attempts(const std::vector<std::string>& attempts, const std::string& solution) {
    WordleAssistant wa(solution.length());
    for (const auto& attempt : attempts) {
        std::string key = attempt_to_key(attempt, solution);
        std::cout << attempt << " -> " << key << "\n" << std::endl;
        std::cout << wa.compatible_with_attempts(attempt) << "\n" << std::endl;
        wa.load_attempt({attempt, key});
        std::cout << domains_to_str(wa.get_domains()) << std::endl;
        std::cout << must_be_present_to_str(wa.get_must_be_present()) << std::endl;
    }
}

int main() {
    /* std::string solution = "darls";
    std::string key;
    std::vector<std::string> attempts {
        "snipe",
        "ghoul",
        "wrack",
        "lards",
        "darls"
    };

    test_attempts(attempts, solution); */

    /// constructor with the length of the word for this instance of the game
 
    /// this is called WordleAssistant to keep it simple
    /// but by "Wordle" we mean the variation on Wordle
    /// that we consider in this assignment
    /// rather than the actual official Wordle
    WordleAssistant wa1(5);
 
    /// infer the meaning of this information from the log of the printing included below
    std::cout << domains_to_str(wa1.get_domains()) << std::endl;
    std::cout << must_be_present_to_str(wa1.get_must_be_present()) << std::endl;
 
    /// preparing input for function load_attempt 
    /// including a word and feedback for that word
    /// the idea is that we played this word and we got this feedback from Wordle or similar
 
    /// I am using on purpose strings that are not actual words in English
    /// (the official Wordle wouldn't let us attempt this as a guess)
    /// to emphasise that we are not concerned with that aspect
    /// as we may be playing a version of Wordle for a different language
    /// or a different dictionary set
 
    /// in the feedback we use these symbols:
    /// # is like grey in Wordle
    /// + is like yellow in Wordle
    /// = is like green in Wordle
 
    std::pair<std::string, std::string> attempt;
    attempt.first = "azyfb";
    attempt.second = "#=++#";
 
    wa1.load_attempt(attempt);
 
    /// consider the log of the printing included below to see how the information has changed
    std::cout << domains_to_str(wa1.get_domains()) << std::endl;
 
    /// function get_must_be_present() must return std::map<char, int>
    /// not std::unordered_map<char, int>
    std::cout << must_be_present_to_str(wa1.get_must_be_present()) << std::endl;
 
    /// a few examples of using the information in order to assess whether
    /// a certain word is compatible with the information we have so far 
    /// this function must return a bool
    /// which will usually be printed as 1 or 0 as we know
    std::cout << wa1.compatible_with_attempts("fzfyx") << std::endl;
    std::cout << wa1.compatible_with_attempts("azyfb") << std::endl;
    std::cout << wa1.compatible_with_attempts("czfcy") << std::endl;
    std::cout << wa1.compatible_with_attempts("fzfcc") << std::endl;
 
    /// we can also directly write like this, no need for an auxiliary std::pair
    wa1.load_attempt({"cccff", "###++"});
 
    std::cout << domains_to_str(wa1.get_domains()) << std::endl;
    std::cout << must_be_present_to_str(wa1.get_must_be_present()) << std::endl;
 
    /// in this case the Wordle-equivalent with which we are playing 
    /// has told us that the word is the correct guess
    wa1.load_attempt({"fzfyx", "====="});
 
    /// we see how the update in information reflects this
    std::cout << domains_to_str(wa1.get_domains()) << std::endl;
    std::cout << must_be_present_to_str(wa1.get_must_be_present()) << std::endl;
 
    /// another instance this time with word of length 3
    WordleAssistant wa2(3);
 
    wa2.load_attempt({"aaa", "###"});
    wa2.load_attempt({"cde", "##+"});
    /// of course we should be able to also load more than one attempt
    /// before accessing information 
 
    std::cout << domains_to_str(wa2.get_domains()) << std::endl;
    std::cout << must_be_present_to_str(wa2.get_must_be_present()) << std::endl;

    
}