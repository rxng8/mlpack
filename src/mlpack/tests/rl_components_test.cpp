/**
 * @file tests/rl_components_test.cpp
 * @author Shangtong Zhang
 * @author Rohan Raj
 *
 * Basic test for the components of reinforcement learning algorithms.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */

#include <mlpack/core.hpp>

#include <mlpack/methods/reinforcement_learning/environment/mountain_car.hpp>
#include <mlpack/methods/reinforcement_learning/environment/continuous_mountain_car.hpp>
#include <mlpack/methods/reinforcement_learning/environment/cart_pole.hpp>
#include <mlpack/methods/reinforcement_learning/environment/double_pole_cart.hpp>
#include <mlpack/methods/reinforcement_learning/environment/continuous_double_pole_cart.hpp>
#include <mlpack/methods/reinforcement_learning/environment/acrobot.hpp>
#include <mlpack/methods/reinforcement_learning/environment/pendulum.hpp>
#include <mlpack/methods/reinforcement_learning/replay/random_replay.hpp>
#include <mlpack/methods/reinforcement_learning/policy/greedy_policy.hpp>
#include <mlpack/methods/reinforcement_learning/environment/frozen_lake.hpp>

#include "catch.hpp"
#include "test_catch_tools.hpp"

using namespace mlpack;
using namespace mlpack::rl;

/**
 * Constructs a Pendulum instance and check if the main routine works as it
 * should be working.
 */
TEST_CASE("SimplePendulumTest", "[RLComponentsTest]")
{
  Pendulum task = Pendulum();
  task.MaxSteps() = 20;

  Pendulum::State state = task.InitialSample();
  Pendulum::Action action;
  action.action[0] = math::Random(-2.0, 2.0);
  double reward, minReward = 0.0;

  REQUIRE(!task.IsTerminal(state));

  while (!task.IsTerminal(state))
  {
    reward = task.Sample(state, action, state);
    minReward = std::min(reward, minReward);
  }

  // The reward is always negative. Check if not lower than lowest possible.
  REQUIRE(minReward >= -(pow(M_PI, 2) + 6.404));

  // Check if the number of steps performed is less or equal as the maximum
  // allowed, since we use a random action there is no guarantee that we will
  // reach the maximum number of steps.
  REQUIRE(task.StepsPerformed() <= 20);

  // The action is simply the torque. Check if dimension is 1.
  REQUIRE(1 == static_cast<size_t>(Pendulum::Action::size));
}

/**
 * Constructs a Continuous MountainCar instance and check if the main rountine
 * works as it should be.
 */
TEST_CASE("SimpleContinuousMountainCarTest", "[RLComponentsTest]")
{
  ContinuousMountainCar task = ContinuousMountainCar();
  task.MaxSteps() = 5;

  ContinuousMountainCar::State state = task.InitialSample();
  ContinuousMountainCar::Action action;
  action.action[0] = math::Random(-1.0, 1.0);
  double reward = task.Sample(state, action);
  // Maximum reward possible is 100.
  REQUIRE(reward <= 100.0);
  REQUIRE(!task.IsTerminal(state));

  while (!task.IsTerminal(state))
    task.Sample(state, action, state);

  // Check if the number of steps performed is the same as the maximum allowed.
  REQUIRE(task.StepsPerformed() == 5);

  // Check if the size of the action space is 1.
  REQUIRE(1 == action.size);
}

/**
 * Constructs a Acrobot instance and check if the main rountine works as
 * it should be.
 */
TEST_CASE("SimpleAcrobotTest", "[RLComponentsTest]")
{
  Acrobot task = Acrobot();
  task.MaxSteps() = 5;

  Acrobot::State state = task.InitialSample();
  Acrobot::Action action;
  action.action = Acrobot::Action::actions::negativeTorque;
  double reward = task.Sample(state, action);

  REQUIRE(reward == -1.0);
  REQUIRE(!task.IsTerminal(state));

  while (!task.IsTerminal(state))
    task.Sample(state, action, state);

  // Check if the number of steps performed is the same as the maximum allowed.
  REQUIRE(task.StepsPerformed() == 5);

  // Check if the size of the action space is 3.
  REQUIRE(3 == static_cast<size_t>(Acrobot::Action::size));
}

/**
 * Constructs a MountainCar instance and check if the main rountine works as
 * it should be.
 */
TEST_CASE("SimpleMountainCarTest", "[RLComponentsTest]")
{
  MountainCar task = MountainCar();
  task.MaxSteps() = 5;

  MountainCar::State state = task.InitialSample();
  MountainCar::Action action;
  action.action = MountainCar::Action::actions::backward;
  double reward = task.Sample(state, action);

  REQUIRE(reward == -1.0);
  REQUIRE(!task.IsTerminal(state));

  while (!task.IsTerminal(state))
    task.Sample(state, action, state);

  // Check if the number of steps performed is the same as the maximum allowed.
  REQUIRE(task.StepsPerformed() == 5);

  // Check if the size of the action space is 3.
  REQUIRE(3 == static_cast<size_t>(MountainCar::Action::size));
}

/**
 * Constructs a CartPole instance and check if the main routine works as
 * it should be.
 */
TEST_CASE("SimpleCartPoleTest", "[RLComponentsTest]")
{
  CartPole task = CartPole();
  task.MaxSteps() = 5;

  CartPole::State state = task.InitialSample();
  CartPole::Action action;
  action.action = CartPole::Action::actions::backward;
  double reward = task.Sample(state, action);

  REQUIRE(reward == 1.0);
  REQUIRE(!task.IsTerminal(state));

  while (!task.IsTerminal(state))
    task.Sample(state, action, state);

  // Check if the number of steps performed is the same as the maximum allowed.
  REQUIRE(task.StepsPerformed() == 5);

  REQUIRE(2 == static_cast<size_t>(CartPole::Action::size));
}

/**
 * Constructs a DoublePoleCart instance and check if the main routine works as
 * it should be.
 */
TEST_CASE("DoublePoleCartTest", "[RLComponentsTest]")
{
  DoublePoleCart task = DoublePoleCart();
  task.MaxSteps() = 5;

  DoublePoleCart::State state = task.InitialSample();
  DoublePoleCart::Action action;
  action.action = DoublePoleCart::Action::actions::backward;
  double reward = task.Sample(state, action);

  REQUIRE(reward == 1.0);
  REQUIRE(!task.IsTerminal(state));

  while (!task.IsTerminal(state))
    task.Sample(state, action, state);

  // Check if the number of steps performed is the same as the maximum allowed.
  REQUIRE(task.StepsPerformed() == 5);
  REQUIRE(2 == static_cast<size_t>(DoublePoleCart::Action::size));
}

/**
 * Constructs a ContinuousDoublePoleCart instance and check if the main
 * routine works as it should be.
 */
TEST_CASE("ContinuousDoublePoleCartTest", "[RLComponentsTest]")
{
  ContinuousDoublePoleCart task = ContinuousDoublePoleCart();
  task.MaxSteps() = 5;

  ContinuousDoublePoleCart::State state = task.InitialSample();
  ContinuousDoublePoleCart::Action action;
  action.action[0] = math::Random(-1.0, 1.0);
  double reward = task.Sample(state, action);

  REQUIRE(reward == 1.0);
  REQUIRE(!task.IsTerminal(state));

  while (!task.IsTerminal(state))
    task.Sample(state, action, state);

  // Check if the number of steps performed is the same as the maximum allowed.
  REQUIRE(task.StepsPerformed() == 5);
  REQUIRE(1 == action.size);
}

/**
 * Construct a random replay instance and check if it works as
 * it should be.
 */
TEST_CASE("RandomReplayTest", "[RLComponentsTest]")
{
  RandomReplay<MountainCar> replay(1, 3);
  MountainCar env;
  MountainCar::State state = env.InitialSample();
  MountainCar::Action action;
  action.action = MountainCar::Action::actions::forward;
  MountainCar::State nextState;
  double reward = env.Sample(state, action, nextState);
  replay.Store(state, action, reward, nextState, env.IsTerminal(nextState),
      0.9);
  arma::mat sampledState;
  std::vector<MountainCar::Action> sampledAction;
  arma::rowvec sampledReward;
  arma::mat sampledNextState;
  arma::irowvec sampledTerminal;

  //! So far there should be only one record in the memory
  replay.Sample(sampledState, sampledAction, sampledReward, sampledNextState,
      sampledTerminal);

  CheckMatrices(state.Encode(), sampledState);
  REQUIRE(sampledAction.size() == 1);
  REQUIRE(action.action == sampledAction[0].action);
  REQUIRE(reward == Approx(arma::as_scalar(sampledReward)).epsilon(1e-7));
  CheckMatrices(nextState.Encode(), sampledNextState);
  REQUIRE(false == arma::as_scalar(sampledTerminal));
  REQUIRE(1 == replay.Size());

  //! Overwrite the memory with a nonsense record
  for (size_t i = 0; i < 5; ++i)
    replay.Store(nextState, action, reward, state, true, 0.9);

  REQUIRE(3 == replay.Size());

  //! Sample several times, the original record shouldn't appear
  for (size_t i = 0; i < 30; ++i)
  {
    replay.Sample(sampledState, sampledAction, sampledReward, sampledNextState,
        sampledTerminal);

    CheckMatrices(state.Encode(), sampledNextState);
    CheckMatrices(nextState.Encode(), sampledState);
    REQUIRE(true == arma::as_scalar(sampledTerminal));
  }
}

/**
 * Construct a greedy policy instance and check if it works as
 * it should be.
 */
TEST_CASE("GreedyPolicyTest", "[RLComponentsTest]")
{
  GreedyPolicy<CartPole> policy(1.0, 10, 0.0, 0.99);
  for (size_t i = 0; i < 15; ++i)
    policy.Anneal();
  REQUIRE(0.0 == Approx(policy.Epsilon()).epsilon(1e-7));
  arma::colvec actionValue = arma::randn<arma::colvec>(CartPole::Action::size);
  CartPole::Action action = policy.Sample(actionValue);
  REQUIRE(actionValue[action.action] ==
      Approx(actionValue.max()).epsilon(1e-7));
}


/**
 * Construct a Frozen Lake instance and check if the main routine
 * work as it should be.
 */
TEST_CASE("FrozenLakeTest", "[RLComponentsTest]")
{
  /**
   * The board and the winning solution will look like this:
   *          S   F   H   F 
   *          |
   *          F   H   F   F 
   *          | 
   *          F---F---F   F 
   *                  |
   *          F   F   F---G 
   * 
   * Similarly, the board and the failing solution will look like this:
   *          S   F   H   F 
   *          |
   *          F   H---F   F 
   *          |       |
   *          F---F---F   F 
   *                  
   *          F   F   F   G 
   */

  // Action definitions.
  FrozenLake::Action goDown;
  goDown.action = FrozenLake::Action::actions::Down;
  FrozenLake::Action goUp;
  goUp.action = FrozenLake::Action::actions::Up;
  FrozenLake::Action goLeft;
  goLeft.action = FrozenLake::Action::actions::Left;
  FrozenLake::Action goRight;
  goRight.action = FrozenLake::Action::actions::Right;

  // Static board definition.
  std::vector<std::vector<char>> sampleBoard {{'S','F','H','F'},
                                              {'F','H','F','F'},
                                              {'F','F','F','F'},
                                              {'F','F','F','G'}};

  // Task 1 checks the case that the agent makes it to the goal,
  // receiving 1.0 as the reward.
  FrozenLake task1 = FrozenLake();
  FrozenLake::State state1 = task1.InitialSample(sampleBoard, 4, 4);
  task1.Sample(state1, goDown, state1);
  task1.Sample(state1, goDown, state1);
  task1.Sample(state1, goRight, state1);
  task1.Sample(state1, goRight, state1);
  task1.Sample(state1, goDown, state1);
  double reward1 = task1.Sample(state1, goRight, state1);

  // Reward must be 1.0 as the agent reaches the goal.
  REQUIRE(reward1 == 1.0);
  // As the agent reaches the goal, the game must end.
  REQUIRE(task1.IsTerminal(state1));

  // Task 2 checks the case where the agent does not
  // complete the game by reaching the max number of steps.
  FrozenLake task2 = FrozenLake();
  FrozenLake::State state2 = task2.InitialSample(sampleBoard, 4, 4);
  task2.MaxSteps() = 10;
  double reward2;
  while (!task2.IsTerminal(state2))
    reward2 = task2.Sample(state2, goDown, state2);

  // Check if the number of steps performed is the same as 
  // the maximum allowed.
  REQUIRE(task2.StepsPerformed() == 10);
  REQUIRE(reward2 == 0.0);

  // Task 3 checks the case where the agent fall into a hole.
  // and recieved -1.0 as a reward.
  FrozenLake task3 = FrozenLake();
  FrozenLake::State state3 = task3.InitialSample(sampleBoard, 4, 4);
  task3.Sample(state3, goDown, state3);
  task3.Sample(state3, goDown, state3);
  task3.Sample(state3, goRight, state3);
  task3.Sample(state3, goRight, state3);
  task3.Sample(state3, goUp, state3);
  double reward3 = task3.Sample(state3, goLeft, state3);

  // Reward must be -1.0 as the agent fall into a hole.
  REQUIRE(reward3 == -1.0);
  // As the agent fall into the hole, the game must end.
  REQUIRE(task3.IsTerminal(state3));

  // Task 4 checks the case with random board.
  FrozenLake task4 = FrozenLake();
  FrozenLake::State state4 = task4.InitialSample();
  while (!task4.IsTerminal(state4))
  { 
    task4.Sample(state4, goDown, state4);
  }
  REQUIRE(task4.IsTerminal(state4));

  // Other checks.
  REQUIRE(4 == static_cast<size_t>(FrozenLake::Action::size));
} 