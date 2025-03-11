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
                attempt_correct[l] = attempt_correct[l] + 1;
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
            std::map<char, int> solution_map = word_to_map(solution);
            char l = attempt.first[i];
            char l_status = attempt.second[i];

            if(l_status = '#') {
                if(must_be_present.find(l) == must_be_present.end() || must_be_present[l] == solution_map[l]) {
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

int main() {

    std::string solution = "bando";
 
    WordleAssistant wa1(5);
    
    wa1.load_attempt({"aayaz", attempt_to_key("aayaz", solution)});
    
    std::cout << domains_to_str(wa1.get_domains()) << std::endl;
    std::cout << must_be_present_to_str(wa1.get_must_be_present()) << std::endl;

    std::cout << wa1.solution << std::endl;
    
    wa1.load_attempt({"eaoao", attempt_to_key("eaoao", solution)});
    
    std::cout << domains_to_str(wa1.get_domains()) << std::endl;
    std::cout << must_be_present_to_str(wa1.get_must_be_present()) << std::endl;

    std::cout << wa1.solution << std::endl;
    

}