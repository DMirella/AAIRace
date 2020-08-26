#ifndef AAIRACE_SOURCE_ENEMY_AI_H_
#define AAIRACE_SOURCE_ENEMY_AI_H_

#include <vector>

#include "ai.h"
#include "ai_types.h"

class EnemyAI {
 public:
  EnemyAI();

  void TrainWithData(const std::vector<AIIOData>& aiio_data);
  AIOutputData GetOutputData(const AIInputData& input_data) const;
 private:
  std::vector<double> TransformAIInputDataToVector(const AIInputData& ai_input_data) const;
  std::vector<double> TransformAIOutputDataToVector(const AIOutputData& ai_output_data) const;
  AIOutputData TransformVectorToAIOutputData(const std::vector<double>& vector) const;
  std::vector<AIIOData> GetTrainData(const std::vector<AIIOData>& aiio_data) const;
  bool GetValueFromActionNumber(const AIOutputData& data, int number) const;

  NeuralNetwork neural_network_;
  std::vector<AIIOData> last_aiio_data_;
};

#endif  // AAIRACE_SOURCE_ENEMY_AI_H_