#ifndef __EVALUATIOR_H__
#define __EVALUATIOR_H__

#include <string>
#include <fstream>
#include <algorithm>

template<class inputType = float, class resultType = float>
class Evaluator {
public:

  Evaluator(const std::string& evalName) : m_evaluationName(evalName) {}
   
  void addData(inputType in, resultType res) {
    m_evaluationData.push_back(std::make_pair(in, res));
  }

  void clearData() {
    m_evaluationData.clear();
  }

  void output(std::string path = ".") {
    std::string filePath = path;
    filePath += "/";
    filePath += m_evaluationName;
    filePath += ".txt";

    std::ofstream out(filePath);
    if (!out) {
      std::cout << "Cannot open " << filePath << std::endl;
      return;
    }

    // sort based on the first 
    std::sort(m_evaluationData.begin(), m_evaluationData.end());

    std::vector<std::pair< inputType, resultType > > finalData;
    size_t i = 0, j = 1;
    while (i < m_evaluationData.size()) {
      if (j < m_evaluationData.size() && m_evaluationData[i].first == m_evaluationData[j].first) {
        j++;
      }
      else {

        if (j - i >= 15) {
          resultType result = m_evaluationData[i].second;
          for (size_t k = i + 1; k < j; k++)
            result += m_evaluationData[k].second;
          result /= (j - i);

          finalData.push_back(std::make_pair(m_evaluationData[i].first, result));
        }
        i = j;
        j++;
      }
    }

    for (int i = 0; i < finalData.size(); i++) {
      out << finalData[i].first << " " << finalData[i].second << std::endl;
    }
  }

private:
  std::string m_evaluationName;

  std::vector<std::pair< inputType, resultType > > m_evaluationData;
};


#endif // 
