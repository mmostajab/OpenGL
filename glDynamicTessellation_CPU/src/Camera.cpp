//camera.cpp
#include "camera.h"

using namespace std;

Camera::Camera() {
  camera_mode = FREE;
  camera_up = glm::vec3(0, 1, 0);
  field_of_view = 45 * glm::pi<double>() / 180.0;
  rotation_quaternion = glm::quat(1, 0, 0, 0);
  camera_position_delta = glm::vec3(0, 0, 0);
  camera_scale = .001f;
  max_pitch_rate = 5;
  max_heading_rate = 5;
  move_camera = false;

  camera_pitch = 0.0f;
  camera_heading = 0.0f;

  //camera_ortho_view_plane_size[0] = 28.0;
  //camera_ortho_view_plane_size[1] = 28.0;
  camera_ortho_view_plane_size[0] = 6.0;
  camera_ortho_view_plane_size[1] = 6.0;

  forward_scroll_up   = 1.1f;
  forward_scroll_down = 0.9f;
}

Camera::~Camera() {
}

void Camera::Reset() {
  camera_up = glm::vec3(0, 1, 0);
}

void Camera::Update() {
  camera_direction = glm::normalize(camera_look_at - camera_position);
  //need to set the matrix state. this is only important because lighting doesn't work if this isn't done
  //glMatrixMode(GL_PROJECTION);
  //glLoadIdentity();
  glViewport(viewport_x, viewport_y, window_width, window_height);

  if (camera_mode == ORTHO) {
    //our projection matrix will be an orthogonal one in this case
    //if the values are not floating point, this command does not work properly
    //need to multiply by aspect!!! (otherise will not scale properly)
    projection = glm::ortho(-1.5f * float(aspect), 1.5f * float(aspect), -1.5f, 1.5f, -10.0f, 10.f);
  }
  else if (camera_mode == FREE) {
    // TODO Replaced by ortho
    //projection = glm::perspective(field_of_view, aspect, near_clip, far_clip);
    projection = glm::ortho(-1.5f * float(aspect), 1.5f * float(aspect), -1.5f, 1.5f, -10.0f, 10.f);
    
    //detmine axis for pitch rotation
    glm::vec3 axis = glm::cross(camera_direction, glm::vec3(0, 1, 0));
    camera_up = glm::normalize(-glm::cross(camera_direction, axis));
    //compute quaternion for pitch based on the camera pitch angle
    glm::quat pitch_quat = glm::angleAxis(camera_pitch * glm::pi<float>() / 180.0f, axis);
    //determine heading quaternion from the camera up vector and the heading angle
    glm::quat heading_quat = glm::angleAxis(camera_heading * glm::pi<float>() / 180.0f, camera_up);
    //add the two quaternions
    glm::quat temp = glm::cross(pitch_quat, heading_quat);
    temp = glm::normalize(temp);
    //update the direction from the quaternion
    camera_direction = glm::rotate(temp, camera_direction);
    //add the camera delta
    camera_position += camera_position_delta;
    //set the look at to be infront of the camera
    camera_look_at = camera_position + camera_direction * 1.0f;
    //damping for smooth camera
    camera_heading *= .5;
    camera_pitch *= .5;
    camera_position_delta = camera_position_delta * .8f;
  }
  else if (camera_mode == MODELVIEWER) {
    // TODO Replaced by ortho
    //projection = glm::perspective(field_of_view, aspect, near_clip, far_clip);
    projection = glm::ortho(
      -camera_ortho_view_plane_size[0] / 2.0f * float(aspect), 
       camera_ortho_view_plane_size[0] / 2.0f * float(aspect), 
      -camera_ortho_view_plane_size[1] / 2.0f, 
       camera_ortho_view_plane_size[1] / 2.0f, 
       float(near_clip), float(far_clip));

    //detmine axis for pitch rotation
    glm::vec3 axis = glm::cross(camera_direction, glm::vec3(0, 1, 0));
    camera_up = glm::normalize(-glm::cross(camera_direction, axis));
    //compute quaternion for pitch based on the camera pitch angle
    glm::quat pitch_quat = glm::angleAxis(camera_pitch * glm::pi<float>() / 180.0f, axis);
    //determine heading quaternion from the camera up vector and the heading angle
    glm::quat heading_quat = glm::angleAxis(camera_heading * glm::pi<float>() / 180.0f, camera_up);
    //add the two quaternions
    glm::quat temp = glm::cross(pitch_quat, heading_quat);
    temp = glm::normalize(temp);
    //update the direction from the quaternion
    camera_direction = glm::rotate(temp, camera_direction);
    //add the camera delta
    camera_look_at += camera_position_delta;
    float r = glm::length(camera_look_at - camera_position);
    //set the look at to be infront of the camera
    camera_position = camera_look_at - camera_direction * r;
    //damping for smooth camera
    camera_heading *= .5;
    camera_pitch *= .5;
    camera_position_delta = camera_position_delta * .8f;
  }
  //compute the MVP
  view = glm::lookAt(camera_position, camera_look_at, camera_up);
  model = glm::mat4(1.0f);
  MVP = projection * view * model;
  //glLoadMatrixf(glm::value_ptr(MVP));

  //OffsetFrustum(0, 0);
}

//Setting Functions
void Camera::SetMode(CameraType cam_mode) {
  camera_mode = cam_mode;
  camera_up = glm::vec3(0, 1, 0);
  rotation_quaternion = glm::quat(1, 0, 0, 0);
}

void Camera::SetPosition(glm::vec3 pos) {
  camera_position = pos;
}

void Camera::SetLookAt(glm::vec3 pos) {
  camera_look_at = pos;
}
void Camera::SetFOV(double fov) {
  field_of_view = fov * glm::pi<double>() / 180.0;
}
void Camera::SetViewport(int loc_x, int loc_y, int width, int height) {
  viewport_x = loc_x;
  viewport_y = loc_y;
  window_width = width;
  window_height = height;
  //need to use doubles division here, it will not work otherwise and it is possible to get a zero aspect ratio with integer rounding
  aspect = double(width) / double(height);
  ;
}
void Camera::SetClipping(double near_clip_distance, double far_clip_distance) {
  near_clip = near_clip_distance;
  far_clip = far_clip_distance;
}

void Camera::Move(CameraDirection dir) {
  if (camera_mode == FREE){
    switch (dir){
    case UP:
      camera_position_delta += camera_up * camera_scale;
      break;
    case DOWN:
      camera_position_delta -= camera_up * camera_scale;
      break;
    case LEFT:
      camera_position_delta -= glm::cross(camera_direction, camera_up) * camera_scale;
      break;
    case RIGHT:
      camera_position_delta += glm::cross(camera_direction, camera_up) * camera_scale;
      break;
    case FORWARD:
      camera_position_delta += camera_direction * camera_scale;
      break;
    case BACK:
      camera_position_delta -= camera_direction * camera_scale;
      break;
    }
  }
}
void Camera::MoveForward(int yOffset){
  switch (camera_mode){
  case FREE:
    camera_position_delta += camera_direction * (camera_scale * yOffset * 10);
    break;

  case MODELVIEWER:
    // TODO projection is changed
    //camera_position += camera_direction * (camera_scale * yOffset * 10);
    if (yOffset < 0.0f) {
      camera_ortho_view_plane_size[0] *= forward_scroll_up;
      camera_ortho_view_plane_size[1] *= forward_scroll_up;
    }
    else {
      camera_ortho_view_plane_size[0] *= forward_scroll_down;
      camera_ortho_view_plane_size[1] *= forward_scroll_down;
    }
    break;
  };
}

void Camera::ChangePitch(float degrees) {
  //Check bounds with the max pitch rate so that we aren't moving too fast
  if (degrees < -max_pitch_rate) {
    degrees = -max_pitch_rate;
  }
  else if (degrees > max_pitch_rate) {
    degrees = max_pitch_rate;
  }
  camera_pitch += degrees;

  //Check bounds for the camera pitch
  if (camera_pitch > 360.0f) {
    camera_pitch -= 360.0f;
  }
  else if (camera_pitch < -360.0f) {
    camera_pitch += 360.0f;
  }
}
void Camera::ChangeHeading(float degrees) {
  //Check bounds with the max heading rate so that we aren't moving too fast
  if (degrees < -max_heading_rate) {
    degrees = -max_heading_rate;
  }
  else if (degrees > max_heading_rate) {
    degrees = max_heading_rate;
  }
  //This controls how the heading is changed if the camera is pointed straight up or down
  //The heading delta direction changes
  if ((camera_pitch > 90 && camera_pitch < 270) || (camera_pitch < -90 && camera_pitch > -270)) {
    camera_heading -= degrees;
  }
  else {
    camera_heading += degrees;
  }
  //Check bounds for the camera heading
  if (camera_heading > 360.0f) {
    camera_heading -= 360.0f;
  }
  else if (camera_heading < -360.0f) {
    camera_heading += 360.0f;
  }
}
void Camera::Move2D(int x, int y) {
  //compute the mouse delta from the previous mouse position
  glm::vec3 mouse_delta = mouse_position - glm::vec3(x, y, 0);
  //if the camera is moving, meaning that the mouse was clicked and dragged, change the pitch and heading
  //	if (move_camera) {
  ChangeHeading(.08f * mouse_delta.x);
  ChangePitch(.08f * mouse_delta.y);
  //	}
  mouse_position = glm::vec3(x, y, 0);
}

void Camera::SetPos(int button, int state, int x, int y) {
  if (button == 3 && state == MOUSE_DOWN) {
    camera_position_delta += camera_up * .05f;
  } else if (button == 4 && state == MOUSE_DOWN) {
    camera_position_delta -= camera_up * .05f;
  } else if (button == LEFT_BUTTON && state == MOUSE_DOWN) {
    move_camera = true;
  } else if (button == LEFT_BUTTON && state == MOUSE_UP) {
    move_camera = false;
  }
  mouse_position = glm::vec3(x, y, 0);
}

void Camera::OffsetFrustum(int x, int y) {

  float translate_x = x - mouse_position.x;
  float translate_y = y - mouse_position.y;

  camera_direction = glm::normalize(camera_look_at - camera_position);
  glm::vec3 axis = glm::cross(camera_direction, glm::vec3(0, 1, 0));
  camera_up = glm::normalize(-glm::cross(camera_direction, axis));

  camera_position += (-translate_x / 100.0f * axis + translate_y / 100.0f * camera_up);
  camera_look_at  += (-translate_x / 100.0f * axis + translate_y / 100.0f * camera_up);
}

void Camera::SetMousePos(int x, int y) {
  mouse_position = glm::vec3(x, y, 0);
}

CameraType Camera::GetMode() {
  return camera_mode;
}

void Camera::GetViewport(int &loc_x, int &loc_y, int &width, int &height) {
  loc_x = viewport_x;
  loc_y = viewport_y;
  width = window_width;
  height = window_height;
}

void Camera::GetMatricies(glm::mat4 &P, glm::mat4 &V, glm::mat4 &M) {
  P = projection;
  V = view;
  M = model;
}

glm::vec3 Camera::getPosition() const
{
  return camera_position;
}