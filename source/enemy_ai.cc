#include "enemy_ai.h"

void EnemyAI::TrainWithData(const std::vector<AIIOData>& aiio_data) {

}

AIOutputData EnemyAI::GetOutputData(const AIInputData& input_data) const {
  return {0, 0, 0, 1};
}