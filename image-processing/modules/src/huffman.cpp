#include <queue>
#include <iostream>
#include <iomanip> // printCodes
#include <fstream>

#include "huffman.h"
#include "histogram.h"

Huffman::TreeNode::TreeNode(std::pair< int, int > pair) {
  intensity = pair.first;
  frequency = pair.second;
  left = nullptr;
  right = nullptr;
}

Huffman::TreeNode &Huffman::TreeNode::operator=(std::pair< int, int > pair) {
  TreeNode treeNode(pair);
  return treeNode;
}

// Кодирование
void Huffman::encoder(cv::Mat in, std::ofstream &fout) {
  cols =  in.cols; rows = in.rows;
  Histogram histogram(in);
  intensityFreq = histogram.getPeriodicity();

  //  Построение дерева Хаффмана
  buildTree();

  generateCodes(rootNode, std::vector<bool>());

  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      for (auto bit : codeMap[in.at<uchar>(i,j)])
        bits.push_back(bit);

  binaryWrite(fout);
  intensityFreq.clear();
  codeMap.clear();
  bits.clear();
}

// Декодирование
cv::Mat Huffman::decoder(std::ifstream &fin) {
  binaryReadHeader(fin);

  //  Построение дерева Хаффмана
  buildTree();
  generateCodes(rootNode, std::vector<bool>());

  binaryReadCode(fin);

  cv::Mat res(rows, cols, CV_8U);

//  TODO:
//  ПОДУМАТЬ НАД ОБРАТНЫМ ПРЕОБРАЗОВАНИЕМ!
  int k = 0;
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++) {
      auto tmp = rootNode;
      for(;;) {
        tmp = bits[k++] ? *tmp.right : *tmp.left;
        if (tmp.left == nullptr && tmp.right == nullptr) {
          res.at<uchar>(i,j) = static_cast<uchar>(tmp.intensity);
          //std::cout << i << " " << j<< " " <<(int)res.at<uchar>(i,j) << std::endl;
          break;
        }
      }
    }
  return res;
}

void Huffman::buildTree() {
  std::priority_queue<TreeNode, std::vector<TreeNode>,
                      std::greater<> > pairs(intensityFreq.begin(), intensityFreq.end());
  // алгоритм хаффмана построения дерево
  while (pairs.size() > 1) {
    const auto pair1 = pairs.top();
    pairs.pop();
    const auto pair2 = pairs.top();
    pairs.pop();

    // приоритет нового узла будет равен сумме их приоритетов
    std::pair<int, int> r(-1, pair1.frequency + pair2.frequency);
    TreeNode temp(r);
    temp.left = new TreeNode(pair1);
    temp.right = new TreeNode(pair2);
    pairs.push(temp);
  }
  rootNode = pairs.top();
}

void Huffman::generateCodes(const TreeNode& node, std::vector<bool> code) {
  if (&node == nullptr) return;
  //  на листьях сохраняем код
  if (node.left== nullptr && node.right== nullptr) {
    codeMap.insert(std::make_pair(node.intensity, code));
  } else {

    std::vector<bool> leftPref = code;
    leftPref.push_back(false);
    generateCodes(*node.left, leftPref); // 0 если влево

    std::vector<bool> rightPref = code;
    rightPref.push_back(true);
    generateCodes(*node.right, rightPref); // 1 если право
  }
}

void Huffman::binaryWrite(std::ofstream& fout) {
  //  сохранить заголовочные данные
  fout.write(reinterpret_cast<const char*>(&cols), sizeof(cols));
  fout.write(reinterpret_cast<const char*>(&rows), sizeof(rows));

  int clr, cntr;
  size_t numberOfIntensity = intensityFreq.size();

  fout.write(reinterpret_cast<const char*>(&numberOfIntensity), sizeof(numberOfIntensity));

  std::vector<std::pair<int,int>> tmp (intensityFreq.begin(), intensityFreq.end());
  for (auto v = tmp.rbegin(); v!= tmp.rend(); ++v ) {
    clr = v->first;
    cntr = v->second;
    fout.write(reinterpret_cast<const char*>(&clr), sizeof(clr));
    fout.write(reinterpret_cast<const char*>(&cntr), sizeof(cntr));
  }

  //  сохранит исходные коды
  std::vector<bool>::size_type n = bits.size();
  fout.write((const char*)&n, sizeof(std::vector<bool>::size_type));
  for(std::vector<bool>::size_type i = 0; i < n;) {
    unsigned char aggr = 0;
    for(unsigned char mask = 1; mask > 0 && i < n; ++i, mask <<= 1)
      if(bits.at(i))
        aggr |= mask;
    fout.write(reinterpret_cast<const char*>(&aggr), sizeof(unsigned char));
  }
}

void Huffman::binaryReadHeader(std::ifstream &fin) {
  fin.read(reinterpret_cast<char*>(&cols), sizeof(cols));
  fin.read(reinterpret_cast<char*>(&rows), sizeof(rows));

  int clr, cntr;
  size_t numOfColors;

  fin.read(reinterpret_cast<char*>(&numOfColors), sizeof(numOfColors));

  for (size_t i = 0; i < numOfColors; ++i) {
    fin.read(reinterpret_cast<char*>(&clr), sizeof(clr));
    fin.read(reinterpret_cast<char*>(&cntr), sizeof(cntr));
    intensityFreq.insert(std::make_pair(clr, cntr));
  }

}
void Huffman::binaryReadCode(std::ifstream &fin) {
  std::vector<bool>::size_type n;
  fin.read((char*)&n, sizeof(std::vector<bool>::size_type));
  bits.resize(n);
  for(std::vector<bool>::size_type i = 0; i < n;) {
    unsigned char aggr;
    fin.read((char*)&aggr, sizeof(unsigned char));
    for(unsigned char mask = 1; mask > 0 && i < n; ++i, mask <<= 1)
      bits.at(i) = aggr & mask;
  }
  fin.close();
}

void Huffman::printCodes() {
  auto prev = std::cout.fill();
  std::cout << "Huffman encoding map:" << std::endl << std::endl;
  for (const auto &v : codeMap) {
    std::cout  << std::setfill('0') << std::setw(3) << v.first << "   ";
    for (const auto &vv : v.second)
      std::cout << vv;
    std::cout << std::dec << std::endl;
  }
  std::cout.fill(prev);
}

void Huffman::printBits() {
  std::cout<<"Bits code: " << std::endl;
  for (bool bit : bits)
    std::cout << bit;
  std::cout<<std::endl << std::endl;
}

double Huffman::getCompressionRatio() {
  int huffman = 0;
  int original = 0;

  for (auto pair : intensityFreq) {
    huffman+=pair.second*codeMap[pair.first].size();
    original+=pair.second*8;
  }

  double res = 1.0 - static_cast<double >(huffman)/original;
  return res;
}
