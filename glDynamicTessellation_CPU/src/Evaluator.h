#ifndef __EVALUATIOR_H__
#define __EVALUATIOR_H__

#include <string>
#include <fstream>
#include <algorithm>

template<class inputType = float, class resultType = float>
class Evaluator {
public:

  template<class inputType = float, class resultType = float>
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

    //std::vector<inputType, resType> finalData;
    //size_t i = 0; 

    //for (int i = 0; i < m_evaluationData.size() - 1; i++) {
     // if(m_evaluationData)
    //}

    for (int i = 0; i < m_evaluationData.size(); i++) {
      out << m_evaluationData[i].first << " " << m_evaluationData[i].second << std::endl;
    }
  }

private:
  std::string m_evaluationName;

  std::vector<std::pair< inputType, resultType > > m_evaluationData;
};


#endif // 
