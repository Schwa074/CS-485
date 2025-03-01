from enum import Enum

class Direction(Enum):
    LEFT = -1
    RIGHT = 1

class CurrentState(Enum):
    RUNNING_DOWN = 0
    RUNNING_LEFT = 1
    RUNNING_RIGHT = 2
    RUNNING_UP = 3
    IDLE = 4

class AnimationType(Enum):
    REPEATING = 0
    ONESHOT = 1