#define INPUT_COUNT 5     //number of analog inputs 模拟输入的数目
#define VDD 5000.0f       //Analog reference voltage in milivolts
#define PI 3.14159265358979f
 
int an[INPUT_COUNT];      //analog inputs  模拟输入
char firstSample;          //marks first sample 第一次采样的标志
 
struct {                                        
  char inpInvert[INPUT_COUNT];    // bits 0..5 invert input  输入的符号  -1 +1
  int zeroLevel[INPUT_COUNT];     // 0..2 accelerometer zero level (mV) @ 0 G  加速度调零
                                  // 3..5 gyro zero level (mV) @ 0 deg/s  陀螺仪调零
  int inpSens[INPUT_COUNT];       // 0..2 acceleromter input sensitivity (mv/g) 加速度输入灵敏度
                                  // 3..5 gyro input sensitivity (mV/deg/ms)  陀螺仪输入灵敏度
  float wGyro;                          // gyro weight/smooting factor  置信度，表示我们对加速度计和陀螺仪的相信程度
} config;        
//记号“W”表示其中一个轴，比如说RwAcc[0],RwAcc[1],RwAcc[2]代表RxAcc,RyAcc,RzAcc                     
//Notation "w" stands for one of the axes, so for example RwAcc[0],RwAcc[1],RwAcc[2] means RxAcc,RyAcc,RzAcc
//Variables below must be global (their previous value is used in getEstimatedInclination)
//下面的变量必须是全局的，他们先前的值在getEstimatedInclination()函数中被使用
float RwEst[3];     //Rw estimated from combining RwAcc and RwGyro Rw的值是RwAcc和RwGyro估算出来的
unsigned long lastMicros;  
//下面的变量不需要是全局的，但是我们定义来调试使用
//Variables below don't need to be global but we expose them for debug purposes
unsigned long interval; //interval since previous analog samples interval
                        //是我们自先前那个模拟样本以来到现在的间隔时间
float RwAcc[3];         //projection of normalized gravitation force vector on x/y/z axis, as measured by accelerometer
                        //加速度测量的3个轴的值
float RwGyro[3];        //Rw obtained from last estimated value and gyro movement Rw从上一次的预测值和陀螺仪的值中得到
float Awz[2];           //angles between projection of R on XZ/YZ plane and Z axis (deg)
 
void setup() {
  static int i;
  Serial.begin(57600); 
 
  //Setup parameters for Acc_Gyro board, see [url=http://www.gadgetgangster.com/213]http://www.gadgetgangster.com/213[/url]
  for(i=0;i<=2;i++){                  // X,Y,Z axis
    config.zeroLevel[i] = 1650;      // Accelerometer zero level (mV) @ 0 G  加速度计调零
    config.inpSens[i] = 478;         // Accelerometer Sensisitivity mV/g   加速度计灵敏度
  }        
   
  for(i=3;i<=4;i++){
    config.inpSens[i] = 2000;            // Gyro Sensitivity mV/deg/ms    陀螺仪灵敏度
    config.zeroLevel[i] = 1230;     // Gyro Zero Level (mV) @ 0 deg/s  陀螺仪调零
  }
 
  config.inpInvert[0] = 1;  //Acc X
  config.inpInvert[1] = 1;  //Acc Y
  config.inpInvert[2] = 1;  //Acc Z
 
  //Gyro readings are sometimes inverted according to accelerometer coordonate system
  //see [url=http://starlino.com/imu_guide.html]http://starlino.com/imu_guide.html[/url] for discussion
  //also see [url=http://www.gadgetgangster.com/213]http://www.gadgetgangster.com/213[/url] for graphical diagrams
  config.inpInvert[3] = 1;  //Gyro X  
  config.inpInvert[4] = 1;  //Gyro Y
 
  config.wGyro = 10;
   
  firstSample = 1;   //第一次采样标志
}
 
void loop() {
  getEstimatedInclination();
  Serial.print(interval);  //microseconds since last sample, please note that printing more data will increase interval    
  Serial.print(",");       //上次样本后经过的时间，请注意串口输出更多的数据就会增大间隔时间 
  Serial.print(RwAcc[0]);  //Inclination X axis (as measured by accelerometer) 加速度测量的x轴的倾向
  Serial.print(",");
  Serial.print(RwEst[0]);  //Inclination X axis (estimated / filtered) 经过滤波处理的x轴
  Serial.println("");
}
 
void getEstimatedInclination(){
  static int i,w;
  static float tmpf,tmpf2;  
  static unsigned long newMicros; //new timestamp 新的时间戳
  static char signRzGyro;  
 
  //get raw adc readings  得到未加工的数据
  newMicros = micros();       //save the time when sample is taken 当样本数据被采集的时候，记录下这时的时间
  for(i=0;i<INPUT_COUNT;i++) an[i]= analogRead(i); //5个模拟引脚分别读取此时的加速度、陀螺仪读数
   
  //compute interval since last sampling time //计算从上次取值后到现在的时间间隔
  interval = newMicros - lastMicros;    //please note that overflows are ok, since for example 0x0001 - 0x00FE will be equal to 2 
                                        //请注意就算数据溢出了也是ok的，因为0x0001 - 0x00FE的值也是2
  lastMicros = newMicros;               //save for next loop, please note interval will be invalid in first sample but we don't use it
                                        //把lastMicros更新成newMicros，以便下一次的运算。请注意第一次的时间间隔是无效的，但是我们没有使用
   
  //get accelerometer readings in g, gives us RwAcc vector
  for(w=0;w<=2;w++) RwAcc[w] = getInput(w);  //经过调零，灵敏度，正负值，把加速度的值变为g
   
  //normalize vector (convert to a vector with same direction and with length 1)
  normalize3DVector(RwAcc);   //正常化加速度向量，使得3个轴的分量平方和为1，矢量长度为1
   
  if (firstSample)
  {
    for(w=0;w<=2;w++) RwEst[w] = RwAcc[w];    //initialize with accelerometer readings
  }                                         //如果是第一次采样，就把RwEst[5]的值初始化为加速度计的测量值
  else
  {             
    //evaluate RwGyro vector
    if(abs(RwEst[2]) < 0.1)
        {
      //Rz is too small and because it is used as reference for computing Axz, Ayz it's error fluctuations will amplify leading to bad results
      //Rz太小，因为它是计算Axz、Ayz的参照。它的错误波动将会被放大，导致结果出错。
          //in this case skip the gyro data and just use previous estimate
          //在这里跳过陀螺仪的数据，只是用先前的估算结果，下面的else语句中的程序不会执行了，直接计算最终的值。
      for(w=0;w<=2;w++) RwGyro[w] = RwEst[w];
    }
        else
        {
      //get angles between projection of R on ZX/ZY plane and Z axis, based on last RwEst
          //在上一次RwEst值的基础上，在R向量在ZX/ZY平面的投影和Z轴之间得到角度值
      for(w=0;w<=1;w++)
          {
        tmpf = getInput(3 + w);                         //get current gyro rate in deg/ms 得到以deg/ms为单位的角速度
        tmpf *= interval / 1000.0f;                     //get angle change in deg 得到deg为单位的角度改变
        Awz[w] = atan2(RwEst[w],RwEst[2]) * 180 / PI;   //get angle and convert to degrees  得到角度并且转换为degrees(是弧度么？)      
        Awz[w] += tmpf;                                 //get updated angle according to gyro movement 根据陀螺仪的值更新角度
      }
       
      //estimate sign of RzGyro by looking in what qudrant the angle Axz is, 
      //RzGyro is pozitive if  Axz in range -90 ..90 => cos(Awz) >= 0
      signRzGyro = ( cos(Awz[0] * PI / 180) >=0 ) ? 1 : -1;  //根据Awz的值来决定RwGyro的值是否是负
       
      //reverse calculation of RwGyro from Awz angles, for formulas deductions see  [url=http://starlino.com/imu_guide.html]http://starlino.com/imu_guide.html[/url]
      //根据Awz的角度值来计算RwGyro的值     
         for(w=0;w<=1;w++){
        RwGyro[0] = sin(Awz[0] * PI / 180);
        RwGyro[0] /= sqrt( 1 + squared(cos(Awz[0] * PI / 180)) * squared(tan(Awz[1] * PI / 180)) );
        RwGyro[1] = sin(Awz[1] * PI / 180);
        RwGyro[1] /= sqrt( 1 + squared(cos(Awz[1] * PI / 180)) * squared(tan(Awz[0] * PI / 180)) );        
      }
      RwGyro[2] = signRzGyro * sqrt(1 - squared(RwGyro[0]) - squared(RwGyro[1]));
    }
     
    //combine Accelerometer and gyro readings 根据置信度组合加速度和陀螺仪
    for(w=0;w<=2;w++) RwEst[w] = (RwAcc[w] + config.wGyro* RwGyro[w]) / (1 + config.wGyro);
 
    normalize3DVector(RwEst);  
    
  }
   
  firstSample = 0;
}
 
void normalize3DVector(float* vector){
  static float R;  
  R = sqrt(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);
  vector[0] /= R;
  vector[1] /= R;  
  vector[2] /= R;  
}
 
float squared(float x){
  return x*x;
}
 
//Convert ADC value for to physical units see [url=http://starlino.com/imu_guide.html]http://starlino.com/imu_guide.html[/url] for explanation.
//For accelerometer it will return  g  (acceleration) , applies when  xyz = 0..2
//For gyro it will return  deg/ms (rate of rotation)  , applies when xyz = 3..5
float getInput(char i){
  static float tmpf;                //temporary variable
  tmpf = an[i] * VDD / 1023.0f;  //voltage (mV)
  tmpf -= config.zeroLevel[i];  //voltage relative to zero level (mV)
  tmpf /= config.inpSens[i];    //input sensitivity in mV/G(acc) or mV/deg/ms(gyro)
  tmpf *= config.inpInvert[i];  //invert axis value according to configuration 
  return tmpf;                
}
