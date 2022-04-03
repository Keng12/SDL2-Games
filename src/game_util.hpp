#ifndef GAME_UTIL_HPP
#define GAME_UTIL_HPP

#include <algorithm>
#include <array>
#include <execution>
#include <chrono>
#include <getopt.h>
#include <tuple>
#include <iostream>

#include "SDL.h"

#include "sdl2_util/video.hpp"

namespace game
{
    void printHelp()
    {
        std::cout << "Usage: snake [options]\n";
        std::cout << "Options:\n";
        std::string space1 = "   ";
        std::string space2 = "   ";
        std::cout << space1 << "-x, --window-width" << space2 << "Specify window width\n";
        std::cout << space1 << "-h, --window-height" << space2 << "Specify window height\n";
        std::cout << space1 << "-s, --speed" << space2 << "Specify speed as \"0\" (low), \"1\" (medium, default) or \"2\" (high)\n";
        std::cout << space1 << "-f, --fps" << space2 << "Specify FPS\n";
        std::cout << space1 << "-v, --vsync" << space2 << "Toggle to use VSync\n";
        exit(EXIT_SUCCESS);
    }
    int parseInteger(const std::string &integer)
    {
        int w = std::stoi(integer);
        if (w <= 0)
        {
            std::cerr << "Error: Parameter " << integer << " <= 0\n";
            exit(EXIT_FAILURE);
        }
        return w;
    }

    std::tuple<int, int, int, int_least8_t, bool> parseInput(int argc, char *const argv[])
    {
        int c;
        int digit_optind = 0;
        int window_width = 500;
        int window_height = 500;
        int speed = 1;
        int fps = 60;
        bool vsync = true;
        while (1)
        {
            int this_option_optind = optind ? optind : 1;
            int option_index = 0;
            struct option long_options[] = {
                {"window-width", required_argument, nullptr, 'x'},
                {"window-height", required_argument, nullptr, 'y'},
                {"speed", required_argument, nullptr, 's'},
                {"fps", required_argument, nullptr, 'f'},
                {"no-vsync", no_argument, nullptr, 'n'},
                {"help", no_argument, nullptr, 'h'}};
            c = getopt_long(argc, argv, "x:y:s:f:nh",
                            long_options, &option_index);
            if (c == -1)
            {
                break;
            }
            std::string flag{};
            switch (c)
            {
            case 'h':
                printHelp();
                exit(EXIT_SUCCESS);
            case 'x':
                flag = "width";
                window_width = parseInteger(optarg);
                break;
            case 'y':
                            flag = "height";

                window_height = parseInteger(optarg);
                break;
            case 's':
                            flag = "speed";

                speed = parseInteger(optarg);
                break;
            case 'f':
                            flag = "FPS";

                window_height = parseInteger(optarg);
                break;
            case 'n':
                if (optarg){
                    std::cerr << "Use n without argument\n";
                    exit(EXIT_FAILURE);
                }
                vsync = false;
                break;
            case '?':
                printHelp();
                exit(EXIT_FAILURE);
            case ':':
                printHelp();
                exit(EXIT_FAILURE);
            }
        }
        if (optind < argc)
        {
            printf("non-option ARGV-elements: ");
            while (optind < argc)
                printf("%s ", argv[optind++]);
            printf("\n");
        }
        return {1, 2, 2, 60, true};
    }

    template <size_t n_row, size_t n_col>
    std::array<std::array<SDL_Rect, n_col>, n_row> constexpr init_rect(const int cell_width, const int cell_height)
    {
        std::array<std::array<SDL_Rect, n_col>, n_row> rect_array{};
        for (size_t row = 0; row < n_row; row++)
        {
            for (size_t col = 0; col < n_col; col++)
            {
                rect_array.at(row).at(col).w = cell_width;
                rect_array.at(row).at(col).h = cell_height;
                rect_array.at(row).at(col).x = col * cell_width;
                rect_array.at(row).at(col).y = row * cell_height;
            }
        }
        return rect_array;
    }
    template <class T, size_t length>
    std::array<T, length> add_arrays(const std::array<T, length> &array1, const std::array<T, length> &array2)
    {
        std::array<T, length> output_array{};
        std::transform(std::execution::unseq, array1.cbegin(), array1.cend(), array2.cbegin(), output_array.begin(), [&](T elem1, T elem2) -> T
                       { return elem1 + elem2; });
        return output_array;
    }
    template <class T, size_t length>
    std::array<T, length> scale_array(const std::array<T, length> &array1, const T factor)
    {
        std::array<T, length> output{};
        std::for_each(std::execution::unseq, array1.begin(), array1.end(), [&](T elem)
                      {T result = elem * factor; return result; });
        return output;
    }
    template <class T, size_t length>
    T sum_array(const std::array<T, length> &array)
    {
        T sum = 0;
        std::for_each(std::execution::unseq, array.cbegin(), array.cend(), [&](T elem)
                      { sum = sum + elem; });
        return sum;
    }
    template <class T, size_t length>
    bool check_equality_arrays(const std::array<T, length> &array1, const std::array<T, length> &array2)
    {
        bool result = std::equal(std::execution::unseq, array1.cbegin(), array1.cend(), array2.cbegin());
        return result;
    }
}

#endif