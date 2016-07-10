cmake_minimum_required(VERSION 3.3)
project(HyperLPR)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

SET(tiny_cnn_hrds Hyperlpr/tiny_cnn/activations/activation_function.h  Hyperlpr/tiny_cnn/io/cifar10_parser.h  Hyperlpr/tiny_cnn/layers/convolutional_layer.h  Hyperlpr/tiny_cnn/io/display.h  Hyperlpr/tiny_cnn/util/image.h  Hyperlpr/tiny_cnn/layers/layer.h  Hyperlpr/tiny_cnn/lossfunctions/loss_function.h  Hyperlpr/tiny_cnn/io/mnist_parser.h  Hyperlpr/tiny_cnn/optimizers/optimizer.h  Hyperlpr/tiny_cnn/util/product.h  Hyperlpr/tiny_cnn/util/util.h
        Hyperlpr/tiny_cnn/layers/average_pooling_layer.h  Hyperlpr/tiny_cnn/config.h  Hyperlpr/tiny_cnn/util/deform.h Hyperlpr/tiny_cnn/layers/fully_connected_layer.h Hyperlpr/tiny_cnn/layers/input_layer.h  Hyperlpr/tiny_cnn/layers/layers.h  Hyperlpr/tiny_cnn/layers/max_pooling_layer.h  Hyperlpr/tiny_cnn/network.h  Hyperlpr/tiny_cnn/layers/partial_connected_layer.h  Hyperlpr/tiny_cnn/tiny_cnn.h  Hyperlpr/tiny_cnn/util/weight_init.h Hyperlpr/tiny_cnn/layers/arithmetic_layer.h Hyperlpr/tiny_cnn/layers/concat_layer.h Hyperlpr/tiny_cnn/layers/feedforward_layer.h Hyperlpr/tiny_cnn/layers/dropout_layer.h Hyperlpr/tiny_cnn/layers/lrn_layer.h Hyperlpr/tiny_cnn/layers/linear_layer.h Hyperlpr/tiny_cnn/node.h Hyperlpr/tiny_cnn/nodes.h Hyperlpr/tiny_cnn/util/target_cost.h Hyperlpr/tiny_cnn/util/aligned_allocator.h Hyperlpr/tiny_cnn/layers/deconvolutional_layer.h Hyperlpr/tiny_cnn/layers/max_unpooling_layer.h Hyperlpr/tiny_cnn/layers/average_unpooling_layer.h)

set(SOURCE_FILES ${tiny_cnn_hrds}  Hyperlpr/util/config.h Hyperlpr/io/linux_dir.h Hyperlpr/judger/judger.h Hyperlpr/locator/locator.h Hyperlpr/util/operate.h Hyperlpr/locator/searcher.h Hyperlpr/recognizer/segment/chars_segment.h Hyperlpr/recognizer/segment/chars_segment.cpp Hyperlpr/recognizer/chinese/CN_PLATE.h Hyperlpr/recognizer/recongizer.h Hyperlpr/recognizer/segment/core_func.h Hyperlpr/recognizer/segment/core_func.cpp Hyperlpr/hyperlpr.h
        Hyperlpr/plate/plate.h Hyperlpr/composer.h)



set(PROJECT_SOURCE_DIR ./Hyperlpr)


find_package(OPENCV REQUIRED)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
LINK_LIBRARIES(${REQUIRED_LIBRARIES})
set(TEST_FILES ${SOURCE_FILES} examples/main.cpp Hyperlpr/enhancement/retinex/retinex.h examples/evaluate.h)
add_executable(HyperLPR  ${TEST_FILES})
include_directories(${OpenCV_INCLUDE_DIRS} )
target_link_libraries(HyperLPR ${OpenCV_LIBS})

