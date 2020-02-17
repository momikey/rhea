#include "inference/engine.hpp"

namespace rhea { namespace inference {
    using namespace rhea::types;

    TypeEngine::TypeEngine() : visitor(this) {}
}}