#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <numeric>

struct BallPathPoint {
    int row;
    int col;

    BallPathPoint() = delete;

    BallPathPoint(
            int _row,
            int _col
    ) : row(_row),
        col(_col) {}
};

struct Ball {
    int row;
    int col;

    std::vector<std::vector<BallPathPoint>> paths;

    Ball() = delete;

    Ball(
            int _row,
            int _col
    ) : row(_row),
        col(_col) {}

    Ball(const Ball &other) = default;

    Ball(Ball &&other) = default;

    bool operator==(const Ball &t) const {
        return (this->row == t.row && this->col == t.col);
    }

    Ball &operator=(const Ball &other) = default;

    bool operator<(const Ball &rhs) const {
        if (row < rhs.row)
            return true;
        if (rhs.row < row)
            return false;
        return col < rhs.col;
    }

    bool operator>(const Ball &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const Ball &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const Ball &rhs) const {
        return !(*this < rhs);
    }
};

enum LineIsValidReturn {
    LINE_NOT_VALID,
    BALL_IN_HOLE,
    LINE_VALID_NOT_IN_HOLE,
};

LineIsValidReturn check_if_line_is_valid(
        int number_moves,
        std::vector<std::string> &course,
        char new_char,
        std::vector<int> &row_range,
        std::vector<int> &col_range,
        std::vector<BallPathPoint> &current_path,
        Ball &ball_loc
);

bool check_all_directions(
        std::vector<std::string> &course,
        int number_moves,
        Ball &ball,
        int current_row,
        int current_col,
        std::vector<BallPathPoint> &current_path
);

bool find_path_for_next_ball(
        std::vector<std::string> &course,
        Ball &ball_loc
);

int main() {

    //#3
    std::vector<std::string> course{
            "4..XX",
            ".H.H.",
            "...H.",
            ".2..2",
            "....."
    };

    //#4
//    std::vector<std::string> course{
//            "3..H.2",
//            ".2..H.",
//            "..H..H",
//            ".X.2.X",
//            "......",
//            "3..H..",
//    };

    //#5
//    std::vector<std::string> course{
//            ".XXX.5X.",
//            "X.4.X..X",
//            "X4..X3.X",
//            "X...X.X.",
//            ".X.X.H.X",
//            "X.HX...X",
//            "X..X.H.X",
//            ".XH.XXX."
//    };

    //#9
//    std::vector<std::string> course{
//            ".XXX.5XX4H5............4H..3XXH.2.HX3...",
//            "XX4.X..X......3.....HH.2X.....5.....4XX.",
//            "X4..X3.X......H...5.....XXXXXXX2.HX2..H.",
//            "X..XXXXX.....H3.H.X..22X3XXH.X2X...2HHXH",
//            ".X.X.H.X........X3XH.HXX.XXXXX.H..HX..2.",
//            "X.HX.X.X....HH....X3.H.X.....H..XXXX3...",
//            "X..X.H.X.43......XXH....HXX3..H.X2.HX2..",
//            ".XHXXXXX..H3H...H2X.H..3X2..HXX3H.2XXXXH"
//    };

    std::vector<Ball> ball_locs;

    for (int row = 0; row < course.size(); ++row) {
        for (int col = 0; col < course[row].size(); ++col) {
            if (std::isdigit(course[row][col])) {
                ball_locs.emplace_back(Ball{row, col});
            }
        }
    }

    for (const auto &s: ball_locs) {
        std::cout << s.row << ' ' << s.col << '\n';
    }

    for (Ball &ball: ball_locs) {
        find_path_for_next_ball(
                course,
                ball
        );
    }

    int possibilities = 1;
    std::cout << "paths\n";
    for (int i = 0; i < ball_locs.size(); i++) {
        std::cout << "index: " << i << '\n';
        for (const auto &path: ball_locs[i].paths) {
            for (const BallPathPoint &c: path) {
                std::cout << "(" << c.row << ", " << c.col << ") ";
            }
            std::cout << '\n';
        }

        possibilities *= ball_locs[i].paths.size();
    }

    //TODO: too many possibilities 1,875,116,0032
    //TODO: so I can do something like take path 1, exclude all other paths that overlap with it, then try all other possibilities?
    // or more like go to the next path in the list and do the same, then if a ball has zero paths, move to the next possible outcome
    std::cout << "possibilities: " << possibilities << '\n';
//    for (std::string &s: course) {
//        for (char &c: s) {
//            if (c != '<'
//                && c != '>'
//                && c != 'v'
//                && c != '^'
//                    ) {
//                c = '.';
//            }
//        }
//    }
//
//    for (const std::string &s: course) {
//        std::cout << s << '\n';
//    }
}

bool find_path_for_next_ball(
        std::vector<std::string> &course,
        Ball &ball_loc
) {

    std::vector<std::string> course_copy = course;

    const int number_moves = course_copy[ball_loc.row][ball_loc.col] - '0';
    std::vector<BallPathPoint> dummy_path;
    check_all_directions(
            course_copy,
            number_moves,
            ball_loc,
            ball_loc.row,
            ball_loc.col,
            dummy_path
    );

    return false;
}

bool check_all_directions(
        std::vector<std::string> &course,
        const int number_moves,
        Ball &ball,
        const int current_row,
        const int current_col,
        std::vector<BallPathPoint> &current_path
) {
    if (number_moves <= 0) {
        return false;
    }

    if ((current_row - number_moves) >= 0) { //up

        std::vector<BallPathPoint> current_path_copy = current_path;

        std::vector<int> row_range(number_moves + 1);
        std::vector<int> col_range{current_col};
        int starting_num = current_row;
        std::generate(row_range.begin(), row_range.end(), [&] { return starting_num--; });

        std::cout << "up col_range: " << col_range.size() << " row_range: " << row_range.size() << std::endl;
        LineIsValidReturn return_value = check_if_line_is_valid(
                number_moves,
                course,
                '^',
                row_range,
                col_range,
                current_path_copy,
                ball
        );

        switch (return_value) {
            case LINE_NOT_VALID:
                //continue to next check
                break;
            case BALL_IN_HOLE:
                //course variable should be set by function
                ball.paths.emplace_back(current_path_copy);
                break;
            case LINE_VALID_NOT_IN_HOLE:
                throw std::invalid_argument("returned LINE_VALID_NOT_IN_HOLE to check_all_directions 'up'");
        }
    }

    if ((current_row + number_moves) <= course.size() - 1) { //down

        std::vector<BallPathPoint> current_path_copy = current_path;

        std::vector<int> row_range(number_moves + 1);
        std::vector<int> col_range{current_col};
        int starting_num = current_row;
        std::generate(row_range.begin(), row_range.end(), [&] { return starting_num++; });

        std::cout << "down col_range: " << col_range.size() << " row_range: " << row_range.size() << std::endl;
        LineIsValidReturn return_value = check_if_line_is_valid(
                number_moves,
                course,
                'v',
                row_range,
                col_range,
                current_path_copy,
                ball
        );

        switch (return_value) {
            case LINE_NOT_VALID:
                //continue to next check
                break;
            case BALL_IN_HOLE:
                //course variable should be set by function
                ball.paths.emplace_back(current_path_copy);
                break;
            case LINE_VALID_NOT_IN_HOLE:
                throw std::invalid_argument("returned LINE_VALID_NOT_IN_HOLE to check_all_directions 'down'");
        }

    }

    if ((current_col - number_moves) >= 0) { //left

        std::vector<BallPathPoint> current_path_copy = current_path;

        std::vector<int> row_range{current_row};
        std::vector<int> col_range(number_moves + 1);
        int starting_num = current_col;
        std::generate(col_range.begin(), col_range.end(), [&] { return starting_num--; });

        std::cout << "left col_range: " << col_range.size() << " row_range: " << row_range.size() << std::endl;
        LineIsValidReturn return_value = check_if_line_is_valid(
                number_moves,
                course,
                '<',
                row_range,
                col_range,
                current_path_copy,
                ball
        );

        switch (return_value) {
            case LINE_NOT_VALID:
                //continue to next check
                break;
            case BALL_IN_HOLE:
                //course variable should be set by function
                ball.paths.emplace_back(current_path_copy);
                break;
            case LINE_VALID_NOT_IN_HOLE:
                throw std::invalid_argument("returned LINE_VALID_NOT_IN_HOLE to check_all_directions 'left'");
        }

    }

    if ((current_col + number_moves) <= course[0].length() - 1) { //right

        std::vector<BallPathPoint> current_path_copy = current_path;

        std::vector<int> row_range{current_row};
        std::vector<int> col_range(number_moves + 1);
        int starting_num = current_col;
        std::generate(col_range.begin(), col_range.end(), [&] { return starting_num++; });
        std::cout << "right col_range: " << col_range.size() << " row_range: " << row_range.size() << std::endl;

        LineIsValidReturn return_value = check_if_line_is_valid(
                number_moves,
                course,
                '>',
                row_range,
                col_range,
                current_path_copy,
                ball
        );

        switch (return_value) {
            case LINE_NOT_VALID:
                //continue to next check
                break;
            case BALL_IN_HOLE:
                //course variable should be set by function
                ball.paths.emplace_back(current_path_copy);
                break;
            case LINE_VALID_NOT_IN_HOLE:
                throw std::invalid_argument("returned LINE_VALID_NOT_IN_HOLE to check_all_directions 'right'");
        }
    }

    return false;
}

LineIsValidReturn check_if_line_is_valid(
        const int number_moves,
        std::vector<std::string> &course,
        const char new_char,
        std::vector<int> &row_range,
        std::vector<int> &col_range,
        std::vector<BallPathPoint> &current_path,
        Ball &ball_loc
) {

    const int end_row = row_range.back();
    const int end_col = col_range.back();
    LineIsValidReturn return_value = LineIsValidReturn::LINE_VALID_NOT_IN_HOLE;
    for (int row: row_range) {
        for (int col: col_range) {
            const char current_char = course[row][col];
            if (current_char == '.'
                || (std::isdigit(current_char)
                    && row == row_range.front()
                    && col == col_range.front())
                || (current_char == 'X'
                    && (row != end_row
                        || col != end_col))
                    ) {
                continue;
            } else if (current_char == 'H'
                       && row == end_row
                       && col == end_col
                    ) {
                return_value = LineIsValidReturn::BALL_IN_HOLE;
                break;
            } else {
                return LineIsValidReturn::LINE_NOT_VALID;
            }
        }
        if (return_value == LineIsValidReturn::BALL_IN_HOLE) {
            break;
        }
    }

    std::vector<std::string> inner_course_copy = course;

    if (row_range.size() > 1) {
        row_range.pop_back();
    }
    if (col_range.size() > 1) {
        col_range.pop_back();
    }

    for (int row: row_range) {
        for (int col: col_range) {
            inner_course_copy[row][col] = new_char;
        }
    }

    for (int col: col_range) {
        std::cout << col << std::endl;
    }

    if (current_path.empty()) {
        current_path.emplace_back(
                row_range.front(),
                col_range.front()
        );
    }

    current_path.emplace_back(
            end_row,
            end_col
    );

    if (return_value == LineIsValidReturn::LINE_VALID_NOT_IN_HOLE) {
        bool went_in_hole = check_all_directions(
                inner_course_copy,
                number_moves - 1,
                ball_loc,
                end_row,
                end_col,
                current_path
        );

        if (went_in_hole) {
            course = std::move(inner_course_copy);
            return LineIsValidReturn::BALL_IN_HOLE;
        } else {
            return LineIsValidReturn::LINE_NOT_VALID;
        }
    } else {
        inner_course_copy[end_row][end_col] = 'U';
    }

    return return_value;
}

/*
bool find_path_for_next_ball(
        std::vector<std::string> &course,
        std::set<Ball> &remaining_ball_locs
) {

    for (const auto &ball_loc: remaining_ball_locs) {
        std::vector<std::string> course_copy = course;

        const int number_moves = course_copy[ball_loc.row][ball_loc.col] - '0';
        bool success = check_all_directions(
                course_copy,
                number_moves,
                ball_loc
        );

        std::cout << "success: " << success << '\n';

        for (const std::string &s: course_copy) {
            std::cout << s << '\n';
        }

        if (success) {
            if (remaining_ball_locs.size() == 1) {
                course = std::move(course_copy);
                return true;
            }

            std::set<Ball> remaining_ball_locs_copy;
            for (const Ball &ball_loc_copy: remaining_ball_locs) {
                if (ball_loc.row != ball_loc_copy.row || ball_loc.col != ball_loc_copy.col)
                    remaining_ball_locs_copy.insert(ball_loc_copy);
            }

            success = find_path_for_next_ball(
                    course_copy,
                    remaining_ball_locs_copy
            );

            if (success) {
                course = std::move(course_copy);
                return true;
            }
        }
    }
    return false;
}

bool check_all_directions(
        std::vector<std::string> &course,
        const int number_moves,
        const Ball &ball_loc
) {
    if (number_moves <= 0) {
        return false;
    }

    if ((ball_loc.row - number_moves) >= 0) { //up

        std::vector<int> row_range(number_moves + 1);
        std::vector<int> col_range{ball_loc.col};
        int starting_num = ball_loc.row;
        std::generate(row_range.begin(), row_range.end(), [&] { return starting_num--; });

        std::cout << "up col_range: " << col_range.size() << " row_range: " << row_range.size() << std::endl;
        LineIsValidReturn return_value = check_if_line_is_valid(
                number_moves,
                course,
                '^',
                row_range,
                col_range
        );

        switch (return_value) {
            case LINE_NOT_VALID:
                //continue to next check
                break;
            case BALL_IN_HOLE:
                //course variable should be set by function
                return true;
            case LINE_VALID_NOT_IN_HOLE:
                throw std::invalid_argument("returned LINE_VALID_NOT_IN_HOLE to check_all_directions 'up'");
        }
    }

    if ((ball_loc.row + number_moves) <= course.size() - 1) { //down

        std::vector<int> row_range(number_moves + 1);
        std::vector<int> col_range{ball_loc.col};
        int starting_num = ball_loc.row;
        std::generate(row_range.begin(), row_range.end(), [&] { return starting_num++; });

        std::cout << "down col_range: " << col_range.size() << " row_range: " << row_range.size() << std::endl;
        LineIsValidReturn return_value = check_if_line_is_valid(
                number_moves,
                course,
                'v',
                row_range,
                col_range
        );

        switch (return_value) {
            case LINE_NOT_VALID:
                //continue to next check
                break;
            case BALL_IN_HOLE:
                //course variable should be set by function
                return true;
            case LINE_VALID_NOT_IN_HOLE:
                throw std::invalid_argument("returned LINE_VALID_NOT_IN_HOLE to check_all_directions 'down'");
        }
    }

    if ((ball_loc.col - number_moves) >= 0) { //left

        std::vector<int> row_range{ball_loc.row};
        std::vector<int> col_range(number_moves + 1);
        int starting_num = ball_loc.col;
        std::generate(col_range.begin(), col_range.end(), [&] { return starting_num--; });

        std::cout << "left col_range: " << col_range.size() << " row_range: " << row_range.size() << std::endl;
        LineIsValidReturn return_value = check_if_line_is_valid(
                number_moves,
                course,
                '<',
                row_range,
                col_range
        );

        switch (return_value) {
            case LINE_NOT_VALID:
                //continue to next check
                break;
            case BALL_IN_HOLE:
                //course variable should be set by function
                return true;
            case LINE_VALID_NOT_IN_HOLE:
                throw std::invalid_argument("returned LINE_VALID_NOT_IN_HOLE to check_all_directions 'left'");
        }
    }

    if ((ball_loc.col + number_moves) <= course[0].length() - 1) { //right

        std::vector<int> row_range{ball_loc.row};
        std::vector<int> col_range(number_moves + 1);
        int starting_num = ball_loc.col;
        std::generate(col_range.begin(), col_range.end(), [&] { return starting_num++; });
        std::cout << "right col_range: " << col_range.size() << " row_range: " << row_range.size() << std::endl;

        LineIsValidReturn return_value = check_if_line_is_valid(
                number_moves,
                course,
                '>',
                row_range,
                col_range
        );

        switch (return_value) {
            case LINE_NOT_VALID:
                //continue to next check
                break;
            case BALL_IN_HOLE:
                //course variable should be set by function
                return true;
            case LINE_VALID_NOT_IN_HOLE:
                throw std::invalid_argument("returned LINE_VALID_NOT_IN_HOLE to check_all_directions 'right'");
        }
    }

    return false;
}

LineIsValidReturn check_if_line_is_valid(
        const int number_moves,
        std::vector<std::string> &course,
        const char new_char,
        std::vector<int> &row_range,
        std::vector<int> &col_range
) {
    const int end_row = row_range.back();
    const int end_col = col_range.back();
    LineIsValidReturn return_value = LineIsValidReturn::LINE_VALID_NOT_IN_HOLE;
    for (int row: row_range) {
        for (int col: col_range) {
            const char current_char = course[row][col];
            if (current_char == '.'
                || (std::isdigit(current_char)
                    && row == row_range.front()
                    && col == col_range.front())
                || (current_char == 'X'
                    && (row != end_row
                        || col != end_col))
                    ) {
                continue;
            } else if (current_char == 'H'
                       && row == end_row
                       && col == end_col
                    ) {
                return_value = LineIsValidReturn::BALL_IN_HOLE;
                break;
            } else {
                return LineIsValidReturn::LINE_NOT_VALID;
            }
        }
        if (return_value == LineIsValidReturn::BALL_IN_HOLE) {
            break;
        }
    }

    std::vector<std::string> inner_course_copy = course;

    if (row_range.size() > 1) {
        row_range.pop_back();
    }
    if (col_range.size() > 1) {
        col_range.pop_back();
    }

    for (int row: row_range) {
        for (int col: col_range) {
            inner_course_copy[row][col] = new_char;
        }
    }

    if (return_value == LineIsValidReturn::LINE_VALID_NOT_IN_HOLE) {
        bool went_in_hole = check_all_directions(
                inner_course_copy,
                number_moves - 1,
                Ball{
                        end_row,
                        end_col
                }
        );

        if (went_in_hole) {
            course = std::move(inner_course_copy);
            return LineIsValidReturn::BALL_IN_HOLE;
        } else {
            return LineIsValidReturn::LINE_NOT_VALID;
        }
    } else {
        inner_course_copy[end_row][end_col] = 'U';
    }

    course = std::move(inner_course_copy);
    return return_value;
}*/
