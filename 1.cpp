#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <map>
#include <algorithm>
#include <thread>
#include <chrono>
#include <iomanip>

using namespace std;

// ANSI color codes for stunning visuals
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BG_BLUE "\033[44m"
#define BG_GREEN "\033[42m"
#define BG_RED "\033[41m"

struct Question {
    string text;
    vector<string> options;
    int correct;
    string verse;
    int difficulty; // 1=Easy, 2=Medium, 3=Hard
    string category;
};

class BibleQuiz {
private:
    vector<Question> questions;
    int score = 0;
    int streak = 0;
    int maxStreak = 0;
    int totalQuestions = 0;
    map<string, int> categoryScores;
    int hintsUsed = 0;
    int lives = 3;
    vector<double> responseTimes;
    
    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
    
    void printBanner() {
        cout << BOLD << CYAN << "\n";
        cout << "╔════════════════════════════════════════════════════════════╗\n";
        cout << "║           ✝️  DIVINE KNOWLEDGE ARENA  ✝️                  ║\n";
        cout << "║          Test Your Biblical Wisdom & Faith!              ║\n";
        cout << "╚════════════════════════════════════════════════════════════╝\n";
        cout << RESET << "\n";
    }
    
    void typewriterEffect(string text, int delayMs = 30) {
        for (char c : text) {
            cout << c << flush;
            this_thread::sleep_for(chrono::milliseconds(delayMs));
        }
        cout << endl;
    }
    
    void showProgressBar(int current, int total) {
        int barWidth = 40;
        float progress = (float)current / total;
        int pos = barWidth * progress;
        
        cout << YELLOW << "[";
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) cout << "█";
            else cout << "░";
        }
        cout << "] " << int(progress * 100.0) << "% (" << current << "/" << total << ")" << RESET << endl;
    }
    
    void displayStats() {
        cout << BOLD << MAGENTA << "\n┌─────────── YOUR STATS ───────────┐\n" << RESET;
        cout << CYAN << "❤️  Lives: " << BOLD << RED;
        for (int i = 0; i < lives; i++) cout << "♥ ";
        cout << RESET << endl;
        cout << CYAN << "⭐ Score: " << BOLD << GREEN << score << RESET << endl;
        cout << CYAN << "🔥 Streak: " << BOLD << YELLOW << streak;
        if (streak >= 3) cout << " 🔥🔥🔥";
        cout << RESET << endl;
        cout << CYAN << "💡 Hints Used: " << BOLD << WHITE << hintsUsed << RESET << endl;
        cout << BOLD << MAGENTA << "└──────────────────────────────────┘\n" << RESET;
    }
    
    void celebrateCorrect() {
        string celebrations[] = {
            "✨ Hallelujah! ✨",
            "🌟 Blessed wisdom! 🌟",
            "⭐ Divine knowledge! ⭐",
            "🎯 Righteous answer! 🎯",
            "🏆 Faithful scholar! 🏆"
        };
        cout << BOLD << GREEN << "\n" << celebrations[rand() % 5] << "\n" << RESET;
    }
    
    void showSadness() {
        cout << BOLD << RED << "\n💔 Not quite... but keep the faith! 💔\n" << RESET;
    }
    
    void initializeQuestions() {
        // OLD TESTAMENT - correct index is 0-based (0,1,2,3)
        questions.push_back({"In the beginning, God created the heavens and...", 
            {"1. The seas", "2. The earth", "3. The light", "4. The animals"}, 
            1, "Genesis 1:1", 1, "Creation"}); // Answer is option 2 (index 1)
        
        questions.push_back({"Who built the ark to survive the great flood?", 
            {"1. Moses", "2. Abraham", "3. Noah", "4. David"}, 
            2, "Genesis 6-9", 1, "Old Testament Heroes"}); // Answer is option 3 (index 2)
        
        questions.push_back({"Which young shepherd defeated the giant Goliath?", 
            {"1. Samuel", "2. Saul", "3. Jonathan", "4. David"}, 
            3, "1 Samuel 17", 1, "Old Testament Heroes"}); // Answer is option 4 (index 3)
        
        questions.push_back({"How many days and nights did it rain during the flood?", 
            {"1. 7", "2. 30", "3. 40", "4. 100"}, 
            2, "Genesis 7:12", 2, "Biblical Numbers"}); // Answer is option 3 (index 2)
        
        questions.push_back({"Who received the Ten Commandments on Mount Sinai?", 
            {"1. Aaron", "2. Moses", "3. Joshua", "4. Elijah"}, 
            1, "Exodus 20", 1, "The Law"}); // Answer is option 2 (index 1)
        
        // NEW TESTAMENT
        questions.push_back({"In which town was Jesus born?", 
            {"1. Nazareth", "2. Jerusalem", "3. Bethlehem", "4. Capernaum"}, 
            2, "Luke 2:4-7", 1, "Life of Jesus"}); // Answer is option 3 (index 2)
        
        questions.push_back({"How many disciples did Jesus choose?", 
            {"1. 7", "2. 10", "3. 12", "4. 40"}, 
            2, "Luke 6:13", 1, "Disciples"}); // Answer is option 3 (index 2)
        
        questions.push_back({"Who betrayed Jesus for 30 pieces of silver?", 
            {"1. Peter", "2. Judas", "3. Thomas", "4. James"}, 
            1, "Matthew 26:14-15", 1, "Passion Week"}); // Answer is option 2 (index 1)
        
        questions.push_back({"On which day did Jesus rise from the dead?", 
            {"1. Friday", "2. Saturday", "3. Sunday", "4. Monday"}, 
            2, "Matthew 28", 1, "Resurrection"}); // Answer is option 3 (index 2)
        
        questions.push_back({"What is the shortest verse in the Bible?", 
            {"1. Pray always", "2. God is love", "3. Jesus wept", "4. Be holy"}, 
            2, "John 11:35", 2, "Biblical Trivia"}); // Answer is option 3 (index 2)
        
        // HARDER QUESTIONS
        questions.push_back({"How many books are in the New Testament?", 
            {"1. 24", "2. 27", "3. 30", "4. 39"}, 
            1, "Bible Structure", 2, "Biblical Knowledge"}); // Answer is option 2 (index 1)
        
        questions.push_back({"Who was swallowed by a great fish?", 
            {"1. Job", "2. Jonah", "3. Jeremiah", "4. Joel"}, 
            1, "Jonah 1:17", 2, "Old Testament Stories"}); // Answer is option 2 (index 1)
        
        questions.push_back({"What did God create on the first day?", 
            {"1. Land", "2. Water", "3. Light", "4. Sky"}, 
            2, "Genesis 1:3-5", 2, "Creation"}); // Answer is option 3 (index 2)
        
        questions.push_back({"Who was the first king of Israel?", 
            {"1. David", "2. Solomon", "3. Saul", "4. Samuel"}, 
            2, "1 Samuel 10", 3, "Kings of Israel"}); // Answer is option 3 (index 2)
        
        questions.push_back({"What language was most of the Old Testament written in?", 
            {"1. Greek", "2. Latin", "3. Hebrew", "4. Aramaic"}, 
            2, "Biblical Languages", 3, "Biblical Knowledge"}); // Answer is option 3 (index 2)
    }
    
    void useHint(Question &q) {
        if (hintsUsed >= 3) {
            cout << RED << "❌ No more hints available!\n" << RESET;
            return;
        }
        
        hintsUsed++;
        cout << YELLOW << "\n💡 HINT ACTIVATED! Removing 2 wrong answers...\n" << RESET;
        
        vector<int> wrongOpts;
        for (int i = 0; i < 4; i++) {
            if (i != q.correct) wrongOpts.push_back(i);
        }
        
        random_shuffle(wrongOpts.begin(), wrongOpts.end());
        
        cout << CYAN << "Remaining options:\n" << RESET;
        for (int i = 0; i < 4; i++) {
            if (i == q.correct || i == wrongOpts[2]) {
                cout << GREEN << q.options[i] << RESET << endl;
            }
        }
        cout << endl;
    }
    
public:
    BibleQuiz() {
        srand(time(0));
        initializeQuestions();
    }
    
    void startGame() {
        clearScreen();
        printBanner();
        
        cout << BOLD << WHITE << "Welcome, faithful seeker! 🙏\n\n" << RESET;
        typewriterEffect("Prepare yourself for a journey through sacred scripture...", 40);
        this_thread::sleep_for(chrono::seconds(1));
        
        cout << "\n" << CYAN << "Choose your challenge:\n" << RESET;
        cout << GREEN << "1. 📖 Quick Quiz (5 questions)\n" << RESET;
        cout << YELLOW << "2. 📚 Standard Quest (10 questions)\n" << RESET;
        cout << RED << "3. 🔥 Ultimate Trial (All 15 questions)\n" << RESET;
        cout << MAGENTA << "4. 📊 View Leaderboard\n" << RESET;
        cout << WHITE << "\nEnter choice: " << RESET;
        
        int mode;
        cin >> mode;
        
        if (mode == 4) {
            showLeaderboard();
            return;
        }
        
        int numQuestions = (mode == 1) ? 5 : (mode == 2) ? 10 : 15;
        
        clearScreen();
        printBanner();
        
        random_shuffle(questions.begin(), questions.end());
        
        for (int i = 0; i < numQuestions && i < questions.size() && lives > 0; i++) {
            totalQuestions++;
            
            cout << "\n" << string(60, '═') << endl;
            cout << BOLD << BLUE << "Question " << (i + 1) << "/" << numQuestions << RESET;
            
            string diffStr = questions[i].difficulty == 1 ? "Easy" : 
                            questions[i].difficulty == 2 ? "Medium" : "Hard";
            cout << " | " << YELLOW << diffStr << RESET;
            cout << " | " << MAGENTA << questions[i].category << RESET << endl;
            cout << string(60, '═') << endl;
            
            displayStats();
            showProgressBar(i, numQuestions);
            
            cout << "\n" << BOLD << WHITE << questions[i].text << RESET << endl;
            
            for (const auto &opt : questions[i].options) {
                cout << CYAN << opt << RESET << endl;
            }
            
            cout << "\n" << YELLOW << "Type 'h' for hint | Enter answer (1-4): " << RESET;
            
            auto startTime = chrono::high_resolution_clock::now();
            
            string input;
            cin >> input;
            
            if (input == "h" || input == "H") {
                useHint(questions[i]);
                cout << "Now enter your answer (1-4): ";
                cin >> input;
            }
            
            auto endTime = chrono::high_resolution_clock::now();
            double timeTaken = chrono::duration<double>(endTime - startTime).count();
            responseTimes.push_back(timeTaken);
            
            int ans = (input[0] - '0') - 1;
            
            if (ans == questions[i].correct) {
                celebrateCorrect();
                int points = (4 - questions[i].difficulty) * 10;
                if (streak >= 3) points *= 1.5;
                if (timeTaken < 5.0) points += 5;
                
                score += points;
                streak++;
                maxStreak = max(maxStreak, streak);
                categoryScores[questions[i].category]++;
                
                cout << CYAN << "📜 Scripture Reference: " << BOLD << questions[i].verse << RESET << endl;
                cout << GREEN << "💰 +" << points << " points!" << RESET << endl;
            } else {
                showSadness();
                lives--;
                streak = 0;
                
                cout << CYAN << "✝️  The correct answer was: " << BOLD << GREEN 
                     << questions[i].options[questions[i].correct] << RESET << endl;
                cout << CYAN << "📜 Scripture: " << BOLD << questions[i].verse << RESET << endl;
            }
            
            if (lives == 0) {
                cout << BOLD << RED << "\n💀 Game Over! You've lost all lives.\n" << RESET;
                break;
            }
            
            cout << "\n" << WHITE << "Press Enter to continue..." << RESET;
            cin.ignore();
            cin.get();
            clearScreen();
            printBanner();
        }
        
        showFinalResults();
    }
    
    void showFinalResults() {
        clearScreen();
        printBanner();
        
        cout << BOLD << CYAN << "\n╔════════════════════════════════════════════════════════════╗\n";
        cout << "║                    FINAL RESULTS                         ║\n";
        cout << "╚════════════════════════════════════════════════════════════╝\n" << RESET;
        
        cout << "\n" << BOLD << GREEN << "🏆 Final Score: " << score << " points" << RESET << endl;
        cout << YELLOW << "🔥 Max Streak: " << maxStreak << RESET << endl;
        cout << CYAN << "✅ Correct: " << score/10 << "/" << totalQuestions << RESET << endl;
        
        double accuracy = (score > 0) ? (score/10.0 / totalQuestions) * 100 : 0;
        cout << MAGENTA << "🎯 Accuracy: " << fixed << setprecision(1) << accuracy << "%" << RESET << endl;
        
        if (!responseTimes.empty()) {
            double avgTime = 0;
            for (double t : responseTimes) avgTime += t;
            avgTime /= responseTimes.size();
            cout << WHITE << "⚡ Avg Response Time: " << setprecision(2) << avgTime << "s" << RESET << endl;
        }
        
        cout << "\n" << BOLD << BLUE << "Category Performance:\n" << RESET;
        for (auto &cat : categoryScores) {
            cout << CYAN << "  " << cat.first << ": " << GREEN << cat.second << " correct" << RESET << endl;
        }
        
        cout << "\n" << BOLD << YELLOW;
        if (accuracy >= 90) {
            cout << "🌟 BIBLICAL SCHOLAR! Outstanding knowledge! 🌟\n";
        } else if (accuracy >= 75) {
            cout << "⭐ FAITHFUL STUDENT! Excellent work! ⭐\n";
        } else if (accuracy >= 60) {
            cout << "📖 GROWING IN WISDOM! Keep studying! 📖\n";
        } else {
            cout << "🙏 SEEKER OF TRUTH! Continue your journey! 🙏\n";
        }
        cout << RESET;
        
        cout << "\n" << CYAN << "\"Study to show thyself approved...\" - 2 Timothy 2:15\n" << RESET;
    }
    
    void showLeaderboard() {
        clearScreen();
        printBanner();
        cout << BOLD << MAGENTA << "\n📊 HALL OF FAITHFUL SCHOLARS 📊\n" << RESET;
        cout << CYAN << "(Coming soon in multiplayer mode!)\n" << RESET;
        cout << "\nPress Enter to return...";
        cin.ignore();
        cin.get();
    }
};

int main() {
    BibleQuiz game;
    game.startGame();
    
    cout << BOLD << CYAN << "\n✝️  Thank you for playing! God bless! ✝️\n" << RESET;
    
    return 0;
}