#ifndef AAIRACE_SOURCE_ENEMY_AI_H_
#define AAIRACE_SOURCE_ENEMY_AI_H_

#include <vector>

#include "ai_types.h"

class EnemyAI {
 public:
  void TrainWithData(const std::vector<AIIOData>& aiio_data);
  AIOutputData GetOutputData(const AIInputData& input_data) const;
 private:
  
};

#endif  // AAIRACE_SOURCE_ENEMY_AI_H_