// chapter 1: Rudimentary Tensor

// stores double values
// stores one or more positive dimensions
// owns one flat vector of double values
// reports shape, rank (number of dimensions) and number of elements
// rejects mismatched shaoe and data
//reads values using checked multidimensional coordinates

// what are constructors
// what are member variables
// what does std:: do?

// how does move work?

// what is an argument? what is a member variable? and what does it mean to move an argument into a member variable?

#include <cstddef>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <cassert>



class Tensor {

public:
    Tensor(
        std::vector<std::size_t> shape, 
        std::vector<double> data

        // move is a way to prevent copying 
    ) : shape_(std::move(shape)), data_(std::move(data)) {  
    
        // shape_ cannot be empty: not allowing empty tensors (scalars)
        if (shape_.empty()) {
            throw std::invalid_argument("chapter 1 tensors must have at least one dimension");
        } 

        // multiply all dimensions to get the number of elements
        std:: size_t expected_elements = 1;
        for (const std::size_t dimension: shape_) {

            if (dimension == 0) {
                throw std::invalid_argument("chapter 1 tensors dimenions must be positive");
            }
            expected_elements *= dimension;
            // this multiplication can overflow, so we need to check for that
            // when this operation grows larger than what size_t type can represent
            // it will wrap around and become a smaller number 
        }

        if (data_.size() != expected_elements) {
            throw std::invalid_argument("tensor shape does not match its data");
        }

    }

    // get the shape of the tensor (use reference to avoid copying, use const to avoid modifying the shape)
    [[nodiscard]] const std::vector<std::size_t>& get_shape() const noexcept {
        return shape_;
    }

    [[nodiscard]] const std::vector<double>& get_data() const noexcept {
        return data_;
    }

    [[nodiscard]] std::size_t get_rank() const noexcept {
        return shape_.size();
    }

    [[nodiscard]] std::size_t get_num_elements() const noexcept {
        return data_.size();
    }

    [[nodiscard]] double at(const std::vector<std::size_t>& idx) const {

        std::size_t flat_index = 0;
        std::size_t stride = 1;

        // validate if index provided, matches rank
        if (idx.size() != get_rank()){
            throw std::invalid_argument("number of indices must match tensor rank");
        }

        // safely run reverse iteration to prevent buss, because for loop breaks when axis is 0 (no overflow)
        for (std::size_t axis = get_rank(); axis > 0; --axis) {
            const std::size_t current_axis = axis - 1;

            // check to prevent out of range problem
            if (idx[current_axis] >= shape_[current_axis]){
                throw std::out_of_range("tensor index is outside of its dimension");
            }

            flat_index += idx[current_axis] * stride;
            stride *= shape_[current_axis];

            // TODO: handle overflow
        }

        return data_[flat_index];
    }


private: 
    std::vector<std::size_t> shape_; // use this for dimensions
    std::vector<double> data_; // use this for elements
    

};





int main() {

    std::cout << "Test 1\n";
    
    std::vector<std::size_t> shape_1 {2, 3};
    std::vector<double> data_1 {0,1,2,3,4,5};

    Tensor t(
        // {2, 3}, // 2 rows, 3 columns
        // {0, 1, 2, 3, 4, 5}
        shape_1, data_1
    );

    // without &, invalidates const from get_shape()
    // auto& shape = t.get_shape();
    std::cout << "Test 1\n";
    auto& actual_shape = t.get_shape();
    assert(actual_shape == shape_1);

    std::cout << "Test 2\n";
    auto& actual_data = t.get_data();
    assert(actual_data == data_1);

    std::cout << "Test 3\n";
    std::size_t actual_rank = t.get_rank();
    assert(actual_rank == 2);

    std::cout << "Test 4\n";
    std::size_t actual_elem_count = t.get_num_elements();
    assert(actual_elem_count == 6);

    std::cout << "Test 5\n";
    // intialize a tensor with an empty shape
    bool test_5_threw = false;
    try {
        Tensor({}, {});
    } catch (std::invalid_argument& error) {

        // intialize the string to compare the statements
        assert(std::string(error.what()) == "chapter 1 tensors must have at least one dimension");
        test_5_threw = true;
    }
    assert(test_5_threw);

    std::cout << "Test 6\n";
    bool test_6_threw = false;
    try {
        Tensor({1, 0}, {});
    } catch (std::invalid_argument& error) {

        assert(std::string(error.what()) == "chapter 1 tensors dimenions must be positive");
        test_6_threw = true;
    }
    assert(test_6_threw);



    std::cout << "Test 7\n";
    bool test_7_threw = false;
    try {
        Tensor({2, 3}, {1,2,3,});
    } catch (std::invalid_argument& error) {

        assert(std::string(error.what()) == "tensor shape does not match its data");
        test_7_threw = true;
    }
    assert(test_7_threw);


    std::cout << "Test 8\n";
    double val = t.at({1,1});
    assert(val == 4);


    std::cout << "Test 9\n";
    bool test_9_threw = false;
    try {
        double val_10 = t.at({1,2, 3});
    } catch (std::invalid_argument& error) {

        assert(std::string(error.what()) == "number of indices must match tensor rank");
        test_9_threw = true;
    }
    assert(test_9_threw);



    std::cout << "Test 10\n";
    bool test_10_threw = false;
    try {
        double val_10 = t.at({2, 3});
    } catch (std::out_of_range& error) {

        assert(std::string(error.what()) == "tensor index is outside of its dimension");
        test_10_threw = true;
    }
    assert(test_10_threw);
    

    std::cout << "Success!\n";
    return 0;
}
  