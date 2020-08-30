#include "screens.h"

#include <iostream>

#include <game/game_session.h>
#include <game/level_manager.h>
#include <game/user_profile.h>

#include "button.h"
#include "center_align_label.h"
#include "edit_text_box.h"
#include "screen_state_machine.h"

namespace {
std::unique_ptr<Button> GenerateClassicBackButton(const GameWindowContext& game_window_context,
                                                  const Button::OnClickCallback& on_click_callback,
                                                  const std::string& button_title) {
  const int kBackButtonWidth = 140; 
  const int kBackButtonHeight = 45;
  const int kBackButtonXYOffset = 40;
  const int kBackButtonX = game_window_context.screen_width - kBackButtonXYOffset - kBackButtonWidth; 
  const int kBackButtonY = game_window_context.screen_height - kBackButtonXYOffset - kBackButtonHeight;

  return std::make_unique<Button>(kBackButtonX, kBackButtonY, kBackButtonWidth, kBackButtonHeight, 
                                  button_title, on_click_callback, game_window_context.draw_function);
}
}  // namespace

// Screen
Screen::Screen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context)
    : game_window_context_(game_window_context)
    , screen_state_machine_(screen_state_machine) {
}

// ProfileChooseScreen
ProfileChooseScreen::ProfileChooseScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context) 
    : Screen(screen_state_machine, game_window_context) {
  const int kButtonsCount = 2;
  const int kButtonYOffset = 10;
  const int kButtonsWidth = game_window_context_.screen_width / 3;
  const int kButtonsHeight = game_window_context_.screen_height / 8;
  const int kButtonsBlockStartX = game_window_context_.screen_width / 3;
  const int kButtonsBlockStartY = (game_window_context_.screen_height - kButtonsHeight - kButtonYOffset) / 2;
  const std::string kSignInButtonText = "SIGN IN";
  const std::string kSignUpButtonText = "SIGN UP";

  Button::OnClickCallback on_sign_in_button_click_callback = std::bind(&ProfileChooseScreen::OnSignInButtonClick, this); 
  Button::OnClickCallback on_sign_up_button_click_callback = std::bind(&ProfileChooseScreen::OnSignUpButtonClick, this);

  sign_in_button_ = std::make_unique<Button>(kButtonsBlockStartX, kButtonsBlockStartY, kButtonsWidth, kButtonsHeight, 
                                             kSignInButtonText, on_sign_in_button_click_callback, 
                                             game_window_context_.draw_function);
  sign_up_button_ = std::make_unique<Button>(kButtonsBlockStartX, kButtonsBlockStartY + kButtonsHeight + kButtonYOffset, kButtonsWidth, kButtonsHeight, 
                                             kSignUpButtonText, on_sign_up_button_click_callback, 
                                             game_window_context_.draw_function);
}

void ProfileChooseScreen::NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) {
  sign_in_button_->Update(elapsed_time, context);
  sign_up_button_->Update(elapsed_time, context);
}

void ProfileChooseScreen::Draw() {
  sign_in_button_->Draw();
  sign_up_button_->Draw();
}

std::string ProfileChooseScreen::GetScreenName() const {
  return "ProfileChooseScreen";
}

void ProfileChooseScreen::OnSignInButtonClick() {
  screen_state_machine_->SetScreen(std::make_shared<UserLogInScreen>(screen_state_machine_, game_window_context_));
}

void ProfileChooseScreen::OnSignUpButtonClick() {
  screen_state_machine_->SetScreen(std::make_shared<UserRegistrationScreen>(screen_state_machine_, game_window_context_));
}

// UserRegistrationScreen
UserRegistrationScreen::UserRegistrationScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context)
    : Screen(screen_state_machine, game_window_context)
    , ok_button_error_popup_pressed_(false)
    , active_popup_(nullptr) {
  const int kElementsYOffset = 10;
  const int kElementsCount = 3;
  const int kTextEditHeight = game_window_context_.screen_height / 8;
  const int kLabelHeight = game_window_context_.screen_height / 10;
  const int kElementsBlockStartX = game_window_context_.screen_width / 3;
  const int kElementsBlockWidth = game_window_context_.screen_width / 3;
  const int kRegisterButtonWidth = kElementsBlockWidth / 1.5f;
  const int kRegisterButtonHeight = kTextEditHeight / 1.2f;
  const int kElementsBlockStartY 
      = (game_window_context_.screen_height - kTextEditHeight - kLabelHeight - kRegisterButtonHeight - 2 * kElementsYOffset) / 2;
  const int kLabelFontSize = 50;
  const std::string kRegisterButtonText = "Create";
  const std::string kLabelText = "Enter your name:";

  Button::OnClickCallback back_button_click_callback = std::bind(&UserRegistrationScreen::OnBackButtonClick, this);
  back_button_ = GenerateClassicBackButton(game_window_context_, back_button_click_callback, "Back");

  text_label_ = std::make_unique<CenterAlignLabel>(tools::Rectangle(kElementsBlockStartX, kElementsBlockStartY, 
                                                                    kElementsBlockStartX + kElementsBlockWidth, kElementsBlockStartY + kLabelHeight),
                                                   kLabelText, kLabelFontSize, game_window_context_.draw_function);
  name_edit_text_box_ = std::make_unique<EditTextBox>(kElementsBlockStartX, kElementsBlockStartY + kLabelHeight + kElementsYOffset, 
                                             kElementsBlockWidth, kTextEditHeight, game_window_context_.draw_function);
  Button::OnClickCallback on_register_button_click_callback = std::bind(&UserRegistrationScreen::OnRegisterButtonClick, this); 
  register_button_ = std::make_unique<Button>(kElementsBlockStartX + (kElementsBlockWidth - kRegisterButtonWidth) / 2, 
                                              kElementsBlockStartY + kTextEditHeight + kLabelHeight + kElementsYOffset * 2, 
                                              kRegisterButtonWidth, kRegisterButtonHeight, 
                                              kRegisterButtonText, on_register_button_click_callback, 
                                              game_window_context_.draw_function);
}

void UserRegistrationScreen::NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) {
  if (active_popup_.get() == nullptr) {
    text_label_->Update(elapsed_time, context);
    name_edit_text_box_->Update(elapsed_time, context);
    register_button_->Update(elapsed_time, context);
    back_button_->Update(elapsed_time, context);
  } else {
    active_popup_->Update(elapsed_time, context);
    if (ok_button_error_popup_pressed_) {
      active_popup_ = nullptr;
      ok_button_error_popup_pressed_ = false;
    }
  }
}

void UserRegistrationScreen::Draw() {
  text_label_->Draw();
  name_edit_text_box_->Draw();
  register_button_->Draw();
  back_button_->Draw();
  if (active_popup_.get() != nullptr) {
    active_popup_->Draw();
  }
}

std::string UserRegistrationScreen::GetScreenName() const {
  return "UserRegistrationScreen";
}

void UserRegistrationScreen::OnRegisterButtonClick() {
  const int kPopupWidth = game_window_context_.screen_width / 1.5f;
  const int kPopupHeight = game_window_context_.screen_width / 6;
  const int kPopupX = (game_window_context_.screen_width - kPopupWidth) / 2;
  const int kPopupY = (game_window_context_.screen_height - kPopupHeight) / 2;
  const auto entered_string = name_edit_text_box_->entered_string();
  if (entered_string.empty()) {
    const std::string kErrorMessage = "User name can not be empty.";
    active_popup_ = std::make_unique<Popup>(
        kPopupX, kPopupY, kPopupWidth, kPopupHeight, kErrorMessage, 
        [this](){ ok_button_error_popup_pressed_ = true; }, game_window_context_.draw_function);
  } else if (UserProfile::CheckIfConfigExist(entered_string)) {
    const std::string kErrorMessage = "User with this name already exists.";
    active_popup_ = std::make_unique<Popup>(
        kPopupX, kPopupY, kPopupWidth, kPopupHeight, kErrorMessage, 
        [this](){ ok_button_error_popup_pressed_ = true; }, game_window_context_.draw_function);
  } else {
    screen_state_machine_->GetUserProfile().Reset();
    screen_state_machine_->GetUserProfile().SetName(entered_string);
    screen_state_machine_->SetScreen(std::make_shared<MenuScreen>(screen_state_machine_, game_window_context_));
  }
}

void UserRegistrationScreen::OnBackButtonClick() {
  screen_state_machine_->SetScreen(std::make_shared<ProfileChooseScreen>(screen_state_machine_, game_window_context_));
}

// UserLogInScreen
UserLogInScreen::UserLogInScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context)
    : Screen(screen_state_machine, game_window_context)
    , ok_button_error_popup_pressed_(false)
    , active_popup_(nullptr) {
  const int kElementsYOffset = 10;
  const int kElementsCount = 3;
  const int kTextEditHeight = game_window_context_.screen_height / 8;
  const int kLabelHeight = game_window_context_.screen_height / 10;
  const int kElementsBlockStartX = game_window_context_.screen_width / 3;
  const int kElementsBlockWidth = game_window_context_.screen_width / 3;
  const int kRegisterButtonWidth = kElementsBlockWidth / 1.5f;
  const int kRegisterButtonHeight = kTextEditHeight / 1.2f;
  const int kElementsBlockStartY 
      = (game_window_context_.screen_height - kTextEditHeight - kLabelHeight - kRegisterButtonHeight - 2 * kElementsYOffset) / 2;
  const int kLabelFontSize = 50;
  const std::string kRegisterButtonText = "GO!";
  const std::string kLabelText = "Enter your name:";
  
  Button::OnClickCallback back_button_click_callback = std::bind(&UserLogInScreen::OnBackButtonClick, this);
  back_button_ = GenerateClassicBackButton(game_window_context_, back_button_click_callback, "Back");

  text_label_ = std::make_unique<CenterAlignLabel>(tools::Rectangle(kElementsBlockStartX, kElementsBlockStartY, 
                                                                    kElementsBlockStartX + kElementsBlockWidth, kElementsBlockStartY + kLabelHeight),
                                                   kLabelText, kLabelFontSize, game_window_context_.draw_function);
  name_edit_text_box_ = std::make_unique<EditTextBox>(kElementsBlockStartX, kElementsBlockStartY + kLabelHeight + kElementsYOffset, 
                                             kElementsBlockWidth, kTextEditHeight, game_window_context_.draw_function);
  Button::OnClickCallback on_register_button_click_callback = std::bind(&UserLogInScreen::OnLogInButtonClick, this); 
  register_button_ = std::make_unique<Button>(kElementsBlockStartX + (kElementsBlockWidth - kRegisterButtonWidth) / 2, 
                                              kElementsBlockStartY + kTextEditHeight + kLabelHeight + kElementsYOffset * 2, 
                                              kRegisterButtonWidth, kRegisterButtonHeight, 
                                              kRegisterButtonText, on_register_button_click_callback, 
                                              game_window_context_.draw_function);
}

void UserLogInScreen::NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) {
  if (active_popup_.get() == nullptr) {
    text_label_->Update(elapsed_time, context);
    name_edit_text_box_->Update(elapsed_time, context);
    register_button_->Update(elapsed_time, context);
    back_button_->Update(elapsed_time, context);
  } else {
    active_popup_->Update(elapsed_time, context);
    if (ok_button_error_popup_pressed_) {
      active_popup_ = nullptr;
      ok_button_error_popup_pressed_ = false;
    }
  }
}

void UserLogInScreen::Draw() {
  text_label_->Draw();
  name_edit_text_box_->Draw();
  register_button_->Draw();
  back_button_->Draw();
  if (active_popup_.get() != nullptr) {
    active_popup_->Draw();
  }
}

std::string UserLogInScreen::GetScreenName() const {
  return "UserLogInScreen";
}

void UserLogInScreen::OnLogInButtonClick() {
  const int kPopupWidth = game_window_context_.screen_width / 1.5f;
  const int kPopupHeight = game_window_context_.screen_width / 6;
  const int kPopupX = (game_window_context_.screen_width - kPopupWidth) / 2;
  const int kPopupY = (game_window_context_.screen_height - kPopupHeight) / 2;
  const auto entered_string = name_edit_text_box_->entered_string();
  if (entered_string.empty()) {
    const std::string kErrorMessage = "User name can not be empty.";
    active_popup_ = std::make_unique<Popup>(
        kPopupX, kPopupY, kPopupWidth, kPopupHeight, kErrorMessage, 
        [this](){ ok_button_error_popup_pressed_ = true; }, game_window_context_.draw_function);
  } else if (!UserProfile::CheckIfConfigExist(entered_string)) {
    const std::string kErrorMessage = "User with this name is not registered.";
    active_popup_ = std::make_unique<Popup>(
        kPopupX, kPopupY, kPopupWidth, kPopupHeight, kErrorMessage, 
        [this](){ ok_button_error_popup_pressed_ = true; }, game_window_context_.draw_function);
  } else {
    screen_state_machine_->GetUserProfile().Reset();
    screen_state_machine_->GetUserProfile().LoadFromConfigFile(name_edit_text_box_->entered_string());
    screen_state_machine_->SetScreen(std::make_shared<MenuScreen>(screen_state_machine_, game_window_context_));
  }
}

void UserLogInScreen::OnBackButtonClick() {
  screen_state_machine_->SetScreen(std::make_shared<ProfileChooseScreen>(screen_state_machine_, game_window_context_));
}

// MenuScreen
MenuScreen::MenuScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context)
    : Screen(screen_state_machine, game_window_context) {
  const int kButtonsCount = 2;
  const int kButtonsWidth = game_window_context_.screen_width / 3;
  const int kButtonsHeight = game_window_context_.screen_height / 8;
  const int kButtonsBlockStartX = game_window_context_.screen_width / 3;
  const int kButtonsBlockStartY = (game_window_context_.screen_height - kButtonsHeight) / 2;
  const int kButtonYOffset = 10;
  const std::string kStartButtonText = "PLAY";
  const std::string kExitButtonText = "EXIT";
  
  Button::OnClickCallback back_button_click_callback = std::bind(&MenuScreen::OnLogOutButtonClick, this);
  log_out_button_ = GenerateClassicBackButton(game_window_context_, back_button_click_callback, "Log out");

  Button::OnClickCallback on_start_game_button_click_callback = std::bind(&MenuScreen::OnStartGameButtonClick, this); 
  Button::OnClickCallback on_exit_game_button_click_callback = std::bind(&MenuScreen::OnExitGameButtonClick, this);

  start_game_button_ = std::make_unique<Button>(kButtonsBlockStartX, kButtonsBlockStartY, kButtonsWidth, kButtonsHeight, 
                                                kStartButtonText, on_start_game_button_click_callback, 
                                                game_window_context_.draw_function);
  exit_game_button_ = std::make_unique<Button>(kButtonsBlockStartX, kButtonsBlockStartY + kButtonsHeight + kButtonYOffset,
                                               kButtonsWidth, kButtonsHeight, kExitButtonText, on_exit_game_button_click_callback,
                                               game_window_context_.draw_function);
}

void MenuScreen::NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) {
  start_game_button_->Update(elapsed_time, context);
  exit_game_button_->Update(elapsed_time, context);
  log_out_button_->Update(elapsed_time, context);
}

void MenuScreen::Draw() {
  start_game_button_->Draw();
  exit_game_button_->Draw();
  log_out_button_->Draw();
}

std::string MenuScreen::GetScreenName() const {
  return "MenuScreen";
}

void MenuScreen::OnStartGameButtonClick() {
  screen_state_machine_->SetScreen(std::make_shared<LevelChooseScreen>(screen_state_machine_, game_window_context_));
}

void MenuScreen::OnExitGameButtonClick() {
  screen_state_machine_->ExitGame();
}

void MenuScreen::OnLogOutButtonClick() {
  screen_state_machine_->GetUserProfile().SaveToConfigFile();
  screen_state_machine_->SetScreen(std::make_shared<ProfileChooseScreen>(screen_state_machine_, game_window_context_));
}

// LevelChooseScreen
LevelChooseScreen::LevelChooseScreen(ScreenStateMachine* const screen_state_machine, const GameWindowContext& game_window_context)
    : Screen(screen_state_machine, game_window_context) {
  const int kLevelsCount = 30;
  const int kLevelButtonSize = 100;
  const int kLevelButtonXYOffset = 20;
  const int kLevelButtonsBlockXYOffset = 150;
  const int kMaxCountLevelButtonsX 
      = (game_window_context_.screen_width - 2 * kLevelButtonsBlockXYOffset) / (kLevelButtonSize + kLevelButtonXYOffset);
  const int kFontLabelSize = 50;
  const std::string kChooseLavelLabelText = "Choose level:";

  Button::OnClickCallback back_button_click_callback = std::bind(&LevelChooseScreen::OnBackButtonClick, this);
  back_button_ = GenerateClassicBackButton(game_window_context_, back_button_click_callback, "Back");
    
  label_ = std::make_unique<CenterAlignLabel>(tools::Rectangle(0, 0, game_window_context_.screen_width, kLevelButtonsBlockXYOffset),
                                              kChooseLavelLabelText, kFontLabelSize, game_window_context_.draw_function);
  for (int current_level = 1; current_level <= kLevelsCount; current_level++) {
    Button::OnClickCallback on_current_level_button_click_callback = [this, current_level]() {
      OnLevelChoosen(current_level);
    };
    level_button_callback_.push_back(on_current_level_button_click_callback);
    level_buttons_.emplace_back(std::make_unique<Button>(
        kLevelButtonsBlockXYOffset + ((current_level - 1) % kMaxCountLevelButtonsX) * (kLevelButtonSize + kLevelButtonXYOffset),
        kLevelButtonsBlockXYOffset + ((current_level - 1) / kMaxCountLevelButtonsX) * (kLevelButtonSize + kLevelButtonXYOffset),
        kLevelButtonSize,
        kLevelButtonSize,
        std::to_string(current_level),
        on_current_level_button_click_callback,
        game_window_context_.draw_function
    ));
    if (screen_state_machine_->GetUserProfile().GetLevelManager().count_unlocked_level() < current_level) {
      level_buttons_.back()->SetEnable(false);
    }
  }
}

void LevelChooseScreen::NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) {
  for (const auto& it : level_buttons_) {
    it->Update(elapsed_time, context);
  }
  back_button_->Update(elapsed_time, context);
}

void LevelChooseScreen::Draw() {
  label_->Draw();
  for (const auto& it : level_buttons_) {
    it->Draw();
  }
  back_button_->Draw();
}

std::string LevelChooseScreen::GetScreenName() const {
  return "LevelChooseScreen";
}

void LevelChooseScreen::OnLevelChoosen(int level) {
  //screen_state_machine_->SetScreen(std::make_shared<GameScreen>(screen_state_machine_, game_window_context_, level));
  const float kTransitionAnimationTime = 4000.0f;
  screen_state_machine_->SetScreen(std::make_shared<TransitionScreen>(
      screen_state_machine_, game_window_context_, shared_from_this(), 
      std::make_shared<GameScreen>(screen_state_machine_, game_window_context_, level),
      kTransitionAnimationTime));
}

void LevelChooseScreen::OnBackButtonClick() {
  screen_state_machine_->SetScreen(std::make_shared<MenuScreen>(screen_state_machine_, game_window_context_));
}

// GameScreen
GameScreen::GameScreen(ScreenStateMachine* const screen_state_machine, 
                       const GameWindowContext& game_window_context,
                       int game_level)
    : Screen(screen_state_machine, game_window_context)
    , active_game_session_(nullptr) {
  active_game_session_ = screen_state_machine_->GetUserProfile().GetLevelManager().GenerateGameSession(game_level);
}

void GameScreen::NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) {
  active_game_session_->NotifyGameCycleElapsed(elapsed_time, context);
  if (active_game_session_->is_game_session_ended()) {
    const float kTransitionAnimationTime = 6000.0f;
    screen_state_machine_->SetScreen(std::make_shared<TransitionScreen>(
        screen_state_machine_, game_window_context_, shared_from_this(), 
        std::make_shared<LevelChooseScreen>(screen_state_machine_, game_window_context_),
        kTransitionAnimationTime));
  }
}

void GameScreen::Draw() {
  active_game_session_->DrawEntities();
}

std::string GameScreen::GetScreenName() const {
  return "GameScreen";
}

// TransitionScreen
TransitionScreen::TransitionScreen(ScreenStateMachine* const screen_state_machine, 
                                   const GameWindowContext& game_window_context,
                                   const std::shared_ptr<Screen>& current_screen,
                                   const std::shared_ptr<Screen>& next_screen,
                                   float transition_animation_time) 
    : Screen(screen_state_machine, game_window_context)
    , transition_animation_time_(transition_animation_time)
    , summary_transition_time_(0.0f)
    , current_transperent_value_(0.0f)
    , current_rectangle_color_(sf::Color(0, 0, 0, 0))
    , current_screen_(current_screen)
    , next_screen_(next_screen) {
  transition_rectangle_.setPosition(sf::Vector2f(0.0f, 0.0f));
  transition_rectangle_.setSize(sf::Vector2f(game_window_context_.screen_width, game_window_context_.screen_height));
  transition_rectangle_.setFillColor(current_rectangle_color_);
}

void TransitionScreen::NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context) {
  const float kSemiTransitionAnimationTime = transition_animation_time_ / 2.0f;
  const float kSemiTransitionAnimationSpeed = (255.0f / kSemiTransitionAnimationTime);
  summary_transition_time_ += elapsed_time;
  if (summary_transition_time_ <= kSemiTransitionAnimationTime) {
    current_transperent_value_ += kSemiTransitionAnimationSpeed * elapsed_time;
    current_transperent_value_ = std::min(current_transperent_value_, 255.0f);
    current_rectangle_color_.a = current_transperent_value_;
    transition_rectangle_.setFillColor(current_rectangle_color_);
  } else if (summary_transition_time_ <= transition_animation_time_) {
    current_transperent_value_ -= kSemiTransitionAnimationSpeed * elapsed_time;
    current_transperent_value_ = std::max(current_transperent_value_, 0.0f);
    current_rectangle_color_.a = current_transperent_value_;
    transition_rectangle_.setFillColor(current_rectangle_color_);
  } else {
    screen_state_machine_->SetScreen(next_screen_);
  }
}

void TransitionScreen::Draw() {
  const float kSemiTransitionAnimationTime = transition_animation_time_ / 2.0f;
  if (summary_transition_time_ <= kSemiTransitionAnimationTime) {
    current_screen_->Draw();
  } else {
    next_screen_->Draw();
  }
  game_window_context_.draw_function(transition_rectangle_);
}

std::string TransitionScreen::GetScreenName() const {
  return "TransitionScreen";
}