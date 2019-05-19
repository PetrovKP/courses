#ifndef IMAGE_PROCESSING_HUFFMAN_H
#define IMAGE_PROCESSING_HUFFMAN_H

#include <utility>
#include <vector>
#include <unordered_map>
#include <opencv2/imgcodecs/imgcodecs.hpp>

// интенсивность-частота
class Huffman {
 public:
  Huffman() = default;

  void printBits();
  void printCodes();

  //  Получить степень сжатия изображения
  double getCompressionRatio();

  void encoder(cv::Mat in, std::ofstream &fout);
  cv::Mat decoder(std::ifstream &fin);

 private:
  //  Дерево Хаффмана
  struct TreeNode {
    int intensity;
    int frequency;

    TreeNode* left, *right;

    TreeNode& operator = (std::pair< int, int >);
    TreeNode() = default;
    TreeNode(std::pair< int, int >);

    friend bool operator > (const TreeNode& a, const TreeNode& b) {
      return a.frequency > b.frequency;
    }
  } rootNode;
  // ***

  std::vector<bool> bits;
  int rows, cols;

  //  Пара. интенсивность - код (код - массив 0 и 1)
  std::unordered_map<int, std::vector<bool>> codeMap;
  //  Пара. интенсивность - частота
  std::unordered_map<int,int> intensityFreq;

  void binaryWrite(std::ofstream& fout);
  void binaryReadHeader(std::ifstream &fin);
  void binaryReadCode(std::ifstream &fin);

  void buildTree();
  void generateCodes(const TreeNode& node, std::vector<bool> code);
};


#endif //IMAGE_PROCESSING_HUFFMANCOMPRESSION_H
