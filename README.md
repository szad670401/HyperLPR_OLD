# HyperLPR Alpha

基于深度学习的实时车牌识别项目,目前还在开发过程中。
### 特点(已实现和待实现)：
*******************

- [x] 车牌定位，判断，识别模块之间相互独立，支持多种识别方式。可自由调配。
- [x] 实时,一张720p图像识别速度在 40-50ms 左右，可对视频流进行实时识别。
- [x] 支持大角度车牌，倾斜车牌。
- [x] 仅包含头文件，移植方便。
- [x] 使用卷积神经网络识别字符。
- [ ] 混淆字符处理。
- [ ] 前置图像增强。
- [ ] 支持基于深度学习的end-to-end端到端的识别，无需分割车牌字符。





### 依赖:
*************************
+ OpenCV
+ tiny_cnn(已包含)
+ TBB（可选）

### 测试使用方法:
**********************
将Hyperlpr目录移动至所需的项目目录，并且包含"hyperlpr.h"头文件即可。

### 样张测试（数据来源于EasyPR测试集）:
******************
![5](./temp/images/0DBF81A2-B57A-4FED-8B1F-F036DAFD57A2.png)
![5](./temp/images/1A3B2BF8-C5FB-48A6-95F6-B68DEF410CB7.png)
![5](./temp/images/5A5F1145-369F-4CD3-B54D-084E7E03970A.png)
![5](./temp/images/7A3C5870-E179-4E36-9AB4-3DB2A8E7FF86.png)
![5](./temp/images/8FEF4367-0C7E-4DD7-970C-FF112A76327F.png)
![5](./temp/images/22E2077D-FA89-43E1-AE63-FFA329DE6E70.png)
![5](./temp/images/3075A244-370A-4D68-B841-69B59CEDA5E3.png)
![5](./temp/images/6908EB01-4C42-4DBC-8E5E-E8550FA99F4B.png)
![5](./temp/images/86404A03-1F78-4675-8CBC-79629716A8A0.png)
![5](./temp/images/958925FE-A48D-4F88-8097-F3A03EE2458F.png)
![5](./temp/images/6908EB01-D24CA125-71E0-4CCE-AE55-2C1F53BF0685.png)
![5](./temp/images/E49B9CFC-5CFE-4DE1-859C-A66056E108FB.png)
![5](./temp/images/F7CA30A9-1D8D-4713-A2EB-559E222C5CFD.png)
### Anthor:
********************
+ Jack Yu
