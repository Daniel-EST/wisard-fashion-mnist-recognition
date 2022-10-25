#include <iostream>
#include <string>
#include <algorithm>
#include <raylib.h>
#include <map>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include "mnist/mnist_reader.hpp"
#include "mnist/mnist_utils.hpp"
#include "wisardpkg/wisardpkg.hpp"

const int IMAGE_WIDTH = 28;
const int IMAGE_HEIGHT = 28;

const int RESIZE_WIDTH = 400;
const int RESIZE_HEIGHT = 400;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const std::string MNIST_DATA_LOCATION("./data/fashion");

int main() {
    std::cout << "\nLoading Data" << std::endl;
    mnist::MNIST_dataset<std::vector, std::vector<uint8_t>, uint8_t> dataset = 
        mnist::read_dataset<std::vector, std::vector, uint8_t, uint8_t>(MNIST_DATA_LOCATION);
    mnist::binarize_dataset(dataset);

    std::vector<std::vector<int>> X;
    std::vector<std::vector<int>> X_test;
    std::vector<std::string> y;
    std::vector<std::string> y_test;

    std::vector<int> img;
    for(int i{}; i < dataset.training_images.size(); i++)
    {
        for(int j{}; j < dataset.training_images.at(i).size(); j++)
            img.push_back((int) dataset.training_images.at(i).at(j));

        X.push_back(img);
        y.push_back(std::to_string(dataset.training_labels.at(i)));
        img.clear();
    }

    for(int i{}; i < dataset.test_images.size(); i++)
    {
        for(int j{}; j < dataset.test_images.at(i).size(); j++)
            img.push_back((int) dataset.test_images.at(i).at(j));

        X_test.push_back(img);
        y_test.push_back(std::to_string(dataset.test_labels.at(i)));
        img.clear();
    }

    std::cout << "\nTrain" << std::endl;
    wisardpkg::Wisard w(28, {
        {"bleachingActivated", true},
        {"ignoreZero", false},
        {"verbose", true},
        {"base", 2},
        {"searchBestConfidence", true}
    });
    w.train(X, y);

    std::cout << "\nTest" << std::endl;
    std::vector<std::string> out = w.classify(X_test);
    float right{};
    for(int i{}; i < out.size(); i++)
    {
        if(!out.at(i).compare(y_test.at(i)))
            right++;
    }
    
    std::map<std::string,std::vector<int>> mentalImages = w.getMentalImages();

    cv::Mat image(
        IMAGE_WIDTH, IMAGE_HEIGHT, 
        CV_8UC3,
        cv::Scalar(0, 0, 0)
    );

    cv::Mat image_cv_rgb;
    cv::Mat resized_up;

    Image image_raylib;
	Texture2D texture;

    int number{};

    std::map<int, const char*> labels;

    labels[0] = "T-shirt/top";
    labels[1] = "Trouser";
    labels[2] = "Pullover";
    labels[3] = "Dress";
    labels[4] = "Coat";
    labels[5] = "Sandal";
    labels[6] = "Shirt";
    labels[7] = "Sneaker";
    labels[8] = "Bag";
    labels[9] = "Ankle boot";

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Fashion");
    SetTargetFPS(60);
	while (!WindowShouldClose())
	{
        if(IsKeyPressed(KEY_RIGHT))
            number = (number + 1) < 10 ? number + 1 : number;

        if(IsKeyPressed(KEY_LEFT))
            number = (number - 1 > -1) ? number - 1 : number;

        std::vector<int> mentalImage = mentalImages[std::to_string(number)];

        float max = *std::max_element(mentalImage.begin(), mentalImage.end());
        
        for(int y{}; y < IMAGE_WIDTH; y++)
        {
            for(int x{}; x < IMAGE_HEIGHT; x++)
            {
                cv::Vec3b& pixel = image.at<cv::Vec3b>(y, x);
                float color = ((float) mentalImage.at((y * IMAGE_WIDTH) + x) / max) * 255;
                pixel[0] = color;
                pixel[1] = color;
                pixel[2] = color;
            }
        }
    
        cv::resize(image, resized_up, cv::Size(RESIZE_WIDTH, RESIZE_HEIGHT), cv::INTER_LINEAR);
        cv::cvtColor(resized_up, image_cv_rgb, cv::COLOR_BGR2RGB);

        image_raylib.width = image_cv_rgb.cols;
        image_raylib.height = image_cv_rgb.rows;
        image_raylib.format = 4;
        image_raylib.mipmaps = 1;
	    image_raylib.data = (void*)(image_cv_rgb.data);

        texture = LoadTextureFromImage(image_raylib);
        const char* text = TextFormat("Type: %s", labels[number]);
        
		BeginDrawing();
            ClearBackground(DARKBLUE);
            DrawFPS(10, 10);

            DrawText(
                "Fashion MNIST Mental Images", 
                (SCREEN_WIDTH / 2) - (MeasureText("Fashion MNIST Mental Images", 30) / 2), 
                (SCREEN_HEIGHT / 2) - 300, 30,
                RAYWHITE
            );

            DrawRectangle(
                (SCREEN_WIDTH / 2) - (RESIZE_WIDTH / 2) - 15, 
                (SCREEN_HEIGHT / 2) - (RESIZE_HEIGHT / 2) - 15, 
                RESIZE_HEIGHT + 30,
                RESIZE_WIDTH + 30,
                GRAY
            );

            DrawTexture(
                texture, 
                (SCREEN_WIDTH / 2) - (RESIZE_WIDTH / 2), (SCREEN_HEIGHT / 2) - (RESIZE_HEIGHT / 2), 
                RAYWHITE
            );

            DrawText(
                text, 
                (SCREEN_WIDTH / 2) - (MeasureText(text, 24) / 2),
                (SCREEN_HEIGHT / 2) + 250, 
                24, 
                RAYWHITE
            );

            DrawText(
                TextFormat("Accuracy: %.2f", right / out.size()), 
                10, 
                690, 
                20, 
                RAYWHITE
            );

		EndDrawing();
        UnloadTexture(texture);
	}

	CloseWindow();

    return 0;
}
