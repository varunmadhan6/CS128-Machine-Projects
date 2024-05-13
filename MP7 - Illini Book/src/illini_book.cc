#include "illini_book.hpp"

// Your code here!
IlliniBook::IlliniBook(const std::string &people_fpath, const std::string &relations_fpath) {
    std::ifstream ifs_one{people_fpath};
    int uin = 0;
    while(ifs_one >> uin) {
        graph_[uin] = std::vector<std::pair<int, std::string>>();
        vertices_.insert(uin);
    }
    ifs_one.close();
    std::ifstream ifs_two{relations_fpath};
    int partner = 0;
    std::string relationship;
    char comma = 0;
     while(ifs_two >> uin >> comma >> partner >> comma >> relationship) {
        graph_[uin].push_back(std::make_pair(partner, relationship));
        graph_[partner].push_back(std::make_pair(uin, relationship));
    }
    ifs_two.close();
}

bool IlliniBook::AreRelated(int uin_1, int uin_2) const {
    if ((graph_.find(uin_1) == graph_.end()) || (graph_.find(uin_2) == graph_.end())) {
        return false;
    }
    std::queue<int> queue;
    std::set<int> visited;
    queue.push(uin_1);
    visited.insert(uin_1);
    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();
        if (current == uin_2) {
            return true;
        }
        for (const auto& node_pair : graph_.at(current)) {
            auto neighbor = node_pair.first;
            if (visited.find(neighbor) == visited.end()) {
                queue.push(neighbor);
                visited.insert(neighbor);
            }
        }
    }
    return false;
}

bool IlliniBook::AreRelated(int uin_1, int uin_2, const std::string &relationship) const {
    if ((graph_.find(uin_1) == graph_.end()) || (graph_.find(uin_2) == graph_.end())) {
        return false;
    }
    std::queue<std::pair<int, std::string>> queue;
    std::set<std::pair<int, std::string>> visited;
    std::pair<int, std::string> pairing = std::make_pair(uin_1, "");
    queue.push(pairing);
    visited.insert(pairing);
    while (!queue.empty()) {
        auto current = queue.front();
        queue.pop();
        if (current.first == uin_2) {
            return true;
        }
        for (const auto& node_pair : graph_.at(current.first)) {
            auto neighbor = node_pair.first;
            const std::string& relation = node_pair.second;
            if (relation == relationship) {
                std::pair<int, std::string> next = std::make_pair(neighbor, relationship);
                if (visited.find(next) == visited.end()) {
                    queue.push(next);
                    visited.insert(next);
                }
            }
        }
    }
    return false;
}

int IlliniBook::GetRelated(int uin_1, int uin_2) const {
    if ((graph_.find(uin_1) == graph_.end()) || (graph_.find(uin_2) == graph_.end())) {
        return -1;
    }
    std::queue<std::pair<int, int>> queue;
    std::set<int> visited;
    queue.push(std::make_pair(uin_1, 0));
    visited.insert(uin_1);
    while (!queue.empty()) {
        int current = queue.front().first;
        int path_length = queue.front().second;
        queue.pop();
        if (current == uin_2) {
            return path_length;
        }
        for (const auto& node_pair : graph_.at(current)) {
            auto neighbor = node_pair.first;
            if (visited.find(neighbor) == visited.end()) {
                queue.push(std::make_pair(neighbor, path_length + 1));
                visited.insert(neighbor);
            }
        }
    }
    return -1;
}

int IlliniBook::GetRelated(int uin_1, int uin_2, const std::string &relationship) const {
    if ((graph_.find(uin_1) == graph_.end()) || (graph_.find(uin_2) == graph_.end())) {
        return -1;
    }
    std::queue<std::pair<int, int>> queue;
    std::set<std::pair<int, std::string>> visited;
    queue.push(std::make_pair(uin_1, 0));
    visited.insert(std::make_pair(uin_1, ""));
    while (!queue.empty()) {
        int current = queue.front().first;
        int path_length = queue.front().second;
        queue.pop();
        if (current == uin_2) {
            return path_length;
        }
        for (const auto& node_pair : graph_.at(current)) {
            auto neighbor = node_pair.first;
            const std::string& relation = node_pair.second;
            if (relation == relationship) {
                std::pair<int, std::string> next = std::make_pair(neighbor, relation);
                if (visited.find(next) == visited.end()) {
                    queue.push(std::make_pair(neighbor, path_length + 1));
                    visited.insert(next);
                }
            }
        }
    }
    return -1;
}

std::vector<int> IlliniBook::GetSteps(int uin, int n) const {
    std::queue<int> queue;
    std::set<int> visited;
    std::vector<int> nodes_at_specific_distance;
    queue.push(uin);
    visited.insert(uin);
    int level = 0;
    while (!queue.empty() && level <= n) {
        int size_of_queue = queue.size();
        for (int i = 0; i < size_of_queue; ++i) {
            int current_node = queue.front();
            queue.pop();
            if (level == n) {
                nodes_at_specific_distance.push_back(current_node);
                continue;
            }
            for (const auto& edge : graph_.at(current_node)) {
                if (visited.find(edge.first) == visited.end()) {
                    queue.push(edge.first);
                    visited.insert(edge.first);
                }
            }
        }
        level++;
    }
    return nodes_at_specific_distance;
}

size_t IlliniBook::CountGroups() const {
    std::set<int> visited;
    int components = 0;
    for (const auto& pair : graph_) {
        int start_node = pair.first;
        if (visited.find(start_node) == visited.end()) {
            std::queue<int> queue;
            queue.push(start_node);
            visited.insert(start_node);

            while (!queue.empty()) {
                int current = queue.front();
                queue.pop();
                for (auto neighbor : graph_.at(current)) {
                    if (visited.find(neighbor.first) == visited.end()) {
                        visited.insert(neighbor.first);
                        queue.push(neighbor.first);
                    }
                }
            }
            components++;
        }
    }
    return static_cast<size_t>(components);
}

size_t IlliniBook::CountGroups(const std::string &relationship) const {
    std::set<int> visited;
    size_t components = 0;
    for (const auto& pair : graph_) {
        int start_node = pair.first;
        if (visited.find(start_node) == visited.end()) {
            std::queue<int> queue;
            queue.push(start_node);
            visited.insert(start_node);
            while (!queue.empty()) {
                int current = queue.front();
                queue.pop();
                for (const auto& neighbor : graph_.at(current)) {
                    int neighbor_node = neighbor.first;
                    const std::string& relate = neighbor.second;
                    if ((visited.find(neighbor_node) == visited.end()) && (relate == relationship)) {
                        visited.insert(neighbor_node);
                        queue.push(neighbor_node);
                    }
                }
            }
            components++;
        }
    }
    return components;
}

size_t IlliniBook::CountGroups(const std::vector<std::string> &relationships) const {
    size_t components = 0;
    std::set<int> visited;
    for (auto pair : graph_) {
        auto start = pair.first;
        if (visited.find(start) == visited.end()) {
            std::queue<int> queue;
            queue.push(start);
            visited.insert(start);
            Modify(queue, visited, relationships);
            /*
            while (!queue.empty()) {
                int current = queue.front();
                queue.pop();
                for (auto& neighbor : graph_.at(current)) {
                    int neighbor_node = neighbor.first;
                    const std::string& relation = neighbor.second;
                    bool found_relation = false;
                    for (const auto& rel : relationships) {
                        if (rel == relation) {
                            found_relation = true;
                            break;
                        }
                    }
                    if (visited.find(neighbor_node) == visited.end() && found_relation) {
                        visited.insert(neighbor_node);
                        queue.push(neighbor_node);
                    }
                }
            }
            */
            components++;
        }
    }
    return components;
}

void IlliniBook::Modify(std::queue<int>& queue, std::set<int>& visited, const std::vector<std::string> &relationships) const {
    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();
        for (const auto& neighbor : graph_.at(current)) {
            int neighbor_node = neighbor.first;
            const std::string& relation = neighbor.second;
            bool found_relation = false;
            for (const auto& rel : relationships) {
                if (rel == relation) {
                    found_relation = true;
                    break;
                }
            }
            if (visited.find(neighbor_node) == visited.end() && found_relation) {
                visited.insert(neighbor_node);
                queue.push(neighbor_node);
            }
        }
    }
}