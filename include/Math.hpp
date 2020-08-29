/**
 * @copyright (C) 2020 Andreas Bank, andreas.mikael.bank@gmail.com
 * A collection of mathematical function.
 */

namespace Math {

  /**
   * Fast sigmoid, f(x) = x / (1 + abs(x)).
   * Refs: https://en.wikipedia.org/wiki/Sigmoid_function
   *       https://stackoverflow.com/questions/10732027/fast-sigmoid-algorithm
   *
   * @param x The x variable.
   *
   * @return The result of f(x), which is a value between 0.0 and 1.0.
   */
  float fast_sigmoid(float x);

}
