#include "huhu.h"

bool open_serial(std::string serial,int speed)
{
//  usbserial u_serial(serial.c_str(), speed);

  usbserial u_serial;
  return true;
}
bool open_camera(cv::VideoCapture& v_capture, int cam_id = 0)
{
  v_capture = cv::VideoCapture(cam_id);
  if (!v_capture.isOpened())
  {
    qDebug() << "open camera failed.";
    return false;
  }
  return true;
}

#define  DISPLAY_MODE_LOOP_TIMES 18
#define  MAX_PICTURE_PTAH 260
bool display_model(cv::VideoCapture& v_capture, usbserial &u_serial)
{
  std::string commnd;
  std::string commnd_lowercase;
  char pic_name[MAX_PICTURE_PTAH] = { 0 };
  cv::Mat src;
  //TODO 串口指令写入 u_serial.send(commnd,commnd.size());
  commnd.clear();
  commnd = "BDA";
  u_serial.send(const_cast<char *>(commnd.c_str()), commnd.size());
  usleep(300);

  commnd_lowercase.clear();
  commnd_lowercase = "bda";
  u_serial.send(const_cast<char *>(commnd_lowercase.c_str()), commnd_lowercase.size());
  usleep(300);

  for (int i = 0; i < DISPLAY_MODE_LOOP_TIMES; i++)
  {
    commnd.clear();
    commnd = "BCA";
    u_serial.send(const_cast<char *>(commnd.c_str()), commnd.size());
    usleep(300);

    v_capture >> src;
    if (!src.empty())
    {
      qDebug() << "pic is empty.";
      return false;
    }
    sprintf(pic_name,"mode%d.png",i);
    cv::imwrite(pic_name,src);
    commnd_lowercase.clear();
    commnd_lowercase = "bca";
    u_serial.send(const_cast<char *>(commnd_lowercase.c_str()), commnd_lowercase.size());
    usleep(300);
  }
  return true;
}
