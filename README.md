# 系统设计文档——SplatDemo

## 背景介绍

喷射战士（Splatoon）是任天堂制作发行的第三人称油彩动作射击类游戏，其创新有趣的游戏机制与独特酷炫的视觉效果深受玩家喜爱，并取得销售佳绩、拿下多项大奖。

本项目以其为原型，运用图形学课程所学，实现游戏的一部分基本机制与视觉效果。


## 实现内容

### **（一）游戏机制**

1. 人物可在陆地上自由移动和跳跃（非线性）， 人物之间有碰撞检测。
    
2. 人物可以发射油漆弹。
    
3. 油漆弹落到地面有涂色效果。
    
4. 油漆弹（经碰撞检测）可对敌方单位造成伤害。

5. 己方单位可通过潜入己方颜色区域加快移动。

6. 进入敌方颜色区域会有减速效果

### **（二）视觉效果**

1. 基本场景与物体（人物、子弹）的绘制。
    
2. 子弹对地面的涂色效果与油漆质感。

3. 人物被击中时的红闪效果。
    
4. 人物生命值耗尽后的爆裂效果。

5. 人物和子弹的阴影。

6. 人物在移动时身后会产生粒子效果。

### **（三）音乐效果**
1. 背景音乐

2. 发射子弹的音效

3. 人物死亡是爆炸的音效

## 框架设计

### 模块设计

##### 物理引擎模块（PhysicalEngine）

    包括物体的基类定义与碰撞判定。

    物体的基本属性包含位置、重力加速度、颜色、编号（物体编号和所属队伍编号）与存活状态等。

    物体分为方形物体与球形物体，方形/球形物体分别具有不同的属性（长宽高/半径）。

    实现不同类型物体之间的碰撞检测。（基于上述方形和球形物体类）

##### 游戏逻辑模块（GameLogic）

    由物体基类派生出子弹、人物等逻辑物体。

    每轮渲染前调用物理引擎获得碰撞判定信息，进行逻辑物体信息的更新调整。

    例如子弹碰到人物/地板，更新血量，消除子弹，更新物体存储信息等。


##### 渲染器模块（Renderer）

    对提供的物体数据进行处理并渲染出图像。

    例如地板涂色的绘制，人物、子弹等物体的绘制，阴影效果。


##### 游戏窗口模块（GameWindow）

    初始化窗口、初始化并处理鼠标键盘事件，管理照相机，处理游戏逻辑模块、渲染器模块以及照相机的一些交互。

    主循环：获取时间信息，根据照相机信息获取projection, view矩阵，先调用游戏逻辑模块进行逻辑物体信息的更新调整，再渲染器模块进行渲染。


### 主要模块实现

##### 物理引擎模块（PhysicalEngine）

**碰撞检测**

    主要分为球体和立方体的碰撞、立方体和立方体的碰撞。球体和立方体的碰撞较为简单，不在这里展开说明。

    立方体的碰撞：将两个立方体记为A和B，首先以立方体B的中心，将B旋转至与坐标轴平行，同时A跟着旋转，取A中的12条棱，分别调用以下线段与立方体求交的函数，判断棱是否与立方体B相交；再将A和B调换这样做一遍。

```c++
bool PhysicalEngine::intersect(glm::vec3 p1, glm::vec3 p2, glm::vec3 boxPoint, float halfLength) {
    float max_x = boxPoint.x + halfLength;
    float min_x = boxPoint.x - halfLength;
    float max_y = 100.0f;//boxPoint.y + halfLength;
    float min_y = 0.0f;//boxPoint.y - halfLength;
    float max_z = boxPoint.z + halfLength;
    float min_z = boxPoint.z - halfLength;
    glm::vec3 dir = p2 - p1;
    float t0 = 0.0f, t1 = 1.0f;
    if (fabs(dir.x) < eps) {
        if (p1.x > max_x + eps || p1.x < min_x - eps) return false;
    } else {
        float inv_dir = 1.0f / dir.x;
        float t_near = (max_x - p1.x) * inv_dir;
        float t_far = (min_x - p1.x) * inv_dir;
        if (t_near > t_far) swap(t_near, t_far);
        t0 = max(t0, t_near);
        t1 = min(t1, t_far);
        if (t0 > t1 + eps) return false;
    }
    if (fabs(dir.y) < eps) {
        if (p1.y > max_y + eps || p1.y < min_y - eps) return false;
    } else {
        float inv_dir = 1.0f / dir.y;
        float t_near = (max_y - p1.y) * inv_dir;
        float t_far = (min_y - p1.y) * inv_dir;
        if (t_near > t_far) swap(t_near, t_far);
        t0 = max(t0, t_near);
        t1 = min(t1, t_far);
        if (t0 > t1 + eps) return false;
    }
    if (fabs(dir.z) < eps) {
        if (p1.z > max_z + eps || p1.z < min_z - eps) return false;
    } else {
        float inv_dir = 1.0f / dir.z;
        float t_near = (max_z - p1.z) * inv_dir;
        float t_far = (max_z - p1.z) * inv_dir;
        if (t_near > t_far) swap(t_near, t_far);
        t0 = max(t0, t_near);
        t1 = min(t1, t_far);
        if (t0 > t1 + eps) return false;
    }
    return true;
}
```
    线段与立方体判交：由于现在立方体已经与坐标轴平行，因此可以很容易地将线段和立方体分别投影到x、y、z三个轴分别判断。

**非线性移动**

    物体具有体积、质量、位置、位移方向、速度、加速度等属性

    地面和空气有摩擦系数（己方颜色油漆上较小，敌方颜色油漆上较大）

    键盘通过WASD控制玩家移动，视为在短时间内有一定的对应方向的加速度

    物体发生碰撞则直接速度取反（实现方便、但不真实）

    通过获取程序运行时间模拟时间流逝

##### 游戏逻辑模块（GameLogic）

**键盘、鼠标交互**
    
    主要是通过照相机类，来间接控制人物移动。
    
    c键切换模式之后，照相机与人物绑定，照相机的移动、视角变换等操作，转化为对应的人物移动、视角变换。

**子弹射击**

    在发射子弹时，对子弹的起始位置和起始速度做了随机扰动，实现了子弹的散射效果。

    子弹的方向与人物的视角朝向一致。

    子弹的初速度较大，使其符合"哒哒哒"的音效。

**子弹类**

    子弹类有一个专门存放当前未落地的子弹的数组。

    在每一次主循环中调用子弹的更新函数，主要是对上抛运动的一个模拟。

    在子弹生成时，记录了子弹的生成时间，方便进行模拟。

**玩家类**

    玩家的操作有很多，如跳跃、射击、下潜、移动等，每个操作主要就是对当前玩家的状态（比如一些物理属性）进行判断和修改。

    在玩家的更新函数中，根据物理状态和时间流逝，更新玩家的位置等信息。

    当玩家不受控制时，设置了一个傻瓜AI让它随机游走和操作。

##### 渲染器模块（Renderer）

**人物和子弹的绘制**

    人物就是一个立方体，因此直接手工生成一个obj文件即可。
    
    球体的生成利用了之前实验中的球体类，将生成的vertex和element信息存入了obj文件中。

**地板的绘制**

    地板采用正方形网格，用大小为 (301, 301) 的网格覆盖坐标范围 (-50 .. 50, -50 .. 50)。

    正常来说：每个正方形利用两个对角三角形来进行绘制可以绘制完整的网格。

    但是在实际调试中我们发现：如果只绘制每个正方形 3/4 的部分，可以营造出透明地板的效果，而且在视觉上有更好的体验。

```C++
for(int i = 0; i < FLOOR_SIZE; ++i) {
    for (int j = 0; j < FLOOR_SIZE; ++j) {
        int idx = i * (FLOOR_SIZE + 1) + j;
        int idy = idx + 1;
        int idu = idx + (FLOOR_SIZE + 1);
        int idv = idy + (FLOOR_SIZE + 1);

        // a triangle
        floorElement.push_back(idx);
        floorElement.push_back(idy);
        floorElement.push_back(idu);

        // a triangle
//      floorElement.push_back(idu);
//      floorElement.push_back(idv);
//      floorElement.push_back(idy);

//      an amazing effect !!!
        floorElement.push_back(idx);
        floorElement.push_back(idy);
        floorElement.push_back(idv);
    }
}
```

**涂色区域的判定**

    这部分判定从理论上来说是属于地板绘制模块，但是由于这部分比较重要，所以单独来描述。

    对于涂色区域，我们定义了一个外半径(OUT_RADIUS)、一个内半径(IN_RADIUS)。

    当子弹落到地面时，首先通过相应的数学运算将其转化为网格坐标，然后将该网格坐标作为起点进行 BFS。

    设某一个被搜索到的点与初始搜索点距离为 d:

        1. d < IN_RADIUS : 设置该点涂色强度为 1，并且设置地板颜色为子弹颜色
        2. d > OUT_RADIUS : continue
        3. others : 按照 d - IN_RADIUS 的值，将该点涂色强度进行差值处理，并且设置地板颜色为子弹颜色

    对应的 shader 中会依据涂色强度来对涂色进行噪声处理，已达成较为真实的油漆质感。
```c++
    while(!q.empty()){
        BulletPos nowPos = q.front();
        q.pop();

        if(!nowPos.checkPos()) continue;

        // distance between this position and bullet position
        float dis = BulletPos::distance(nowPos, src);

        // check whether need render
        if(dis > OUT_RADIUS*OUT_RADIUS) continue;

        int id = nowPos.mapToId();
        int idStartPos = id * FLOOR_ELEMENT_COUNT;

        float& colorAlpha = floorVertices[idStartPos + 6];
        if(dis <= IN_RADIUS*IN_RADIUS){
            colorAlpha = 1.0f;
        }else{ // calc difference
            float tFactor = 1.0f/ (OUT_RADIUS - IN_RADIUS);
            float d = sqrt(dis);
            float t = 1.0f - (d - IN_RADIUS)*tFactor;

            colorAlpha = std::min(colorAlpha + t, 1.0f);
        }

        floorVertices[idStartPos + 3] = color[0];
        floorVertices[idStartPos + 4] = color[1];
        floorVertices[idStartPos + 5] = color[2];

        // find adjacent
        for(int i = 0;i < 4; ++ i){
            BulletPos nxt(nowPos + BulletPos(dx[i], dy[i]));
            if(s.find(nxt) == s.end()){
                s.insert(nxt);
                q.push(nxt);
            }
        }
    }
```


**光照的实现**

[参考教程(learnOpenGL-高级光照)](https://learnopengl-cn.github.io/05%20Advanced%20Lighting/01%20Advanced%20Lighting/)

    本项目的光照使用了：Blinn-Phong 模型，代码参考了 learnOpenGL 上的教程，按照该教程实现的光照。

    由于本次使用的天空盒有对应的太阳位置，所以光源设定在了天空盒所标识的太阳位置。

    所以在地板为透明时，光照的反射与天空盒下面的水面反射融合在了一起，有较好的视觉效果。

**阴影的实现**

[参考教程(learnOpenGL-阴影映射)](https://learnopengl-cn.github.io/05%20Advanced%20Lighting/03%20Shadows/01%20Shadow%20Mapping/)

    创建新的帧缓冲(深度缓冲)，将视角定位到光源位置，调整参数使得此时的投影矩阵为光源位置看向平面的正交投影矩阵。

    将所有需要渲染阴影的物体渲染进该帧缓冲

    渲染完毕后，将该缓冲的深度缓冲作为纹理传入地板渲染部分

    在地板渲染中将要渲染的点通过相应正交矩阵进行映射，比较该点的深度和纹理中的深度来判断该点是否会被遮盖。

**游戏音效的实现**

    游戏音效使用了跨平台音效库 irrKlang

    在游戏开始时播放 BGM (「シオカラ節」，喷射2 DLC 中与真三号对战的BGM)
    
    在自己操作角色进行射击时播放射击音效，有人物死亡时播放爆炸音效。

**油漆效果的实现**

    对于每个地板的采样点，将提供给shader一个染色强度。shader会根据染色强度采用不同的颜色计算方式。

    为了表现出油漆不规则喷溅的效果，我们引入噪声纹理，将该点的染色强度与噪声强度作差，并设置两个阈值，将差值区间分为三部分：强染色、边缘染色、未染色。

    对于强染色部分，为了表现油漆喷涂的不均匀感，将颜色与噪声进行混合；对于边缘染色部分，再设噪声阈值，若噪声值较大，也根据染色强度与地板颜色进行混合，以产生渐变效果，若噪声较小，则直接取地板颜色，产生不规则喷溅效果；对于未染色部分，直接取地板颜色即可。

**粒子的实现**

    粒子即带有持续时长的物体，可以继承Object类的属性。在本项目中潜地尾迹效果使用了粒子。

    在每帧开始时用间隔时间更新粒子的位置、速度等基本属性与life属性，若life低于0则销毁粒子。

    更改life值即更改粒子的持续时间。可以根据life属性为粒子指定不同的绘制方式与位置变化，从而产生动态效果。

## 部署与运行

本项目使用库：`glew 2.2.0`, `glfw3.3.2`, `irrKlang 1.6`, `glm 0.9.8.5`, `stb_image`

由于我们组有 windows 和 macOS 两种系统，所以我们选择了使用 vscode(windows) + CLion(macOS) ，使用 CmakeLists 的方式来编译项目。

运行时程序需要从外部读取：模型文件、纹理文件与音效文件，为了便于调试，我们在程序中通过间接路径指定了文件路径，所以程序只能在 cmake-build-debug 运行，不然很可能出现找不到文件的错误。

如果需要编译该项目，需要配置 `glew`、`glfw` 以及 `irrKlang 1.6` 的相关环境。

## 操作说明

键盘控制
    
    W : 控制照相机向正前方移动/给玩家向前的速度

    S : 控制照相机向正后方移动/给玩家向后的速度

    A : 控制照相机向左移动/给玩家向左的速度

    D : 控制照相机向右移动/给玩家向右的速度

    C : 切换全局照相机/玩家视角
    
    Q : 切换到上一个玩家（若处于玩家视角）

    E : 切换到下一个玩家（若处于玩家视角）

    Space : 控制照相机向正上方移动/控制玩家跳跃

    Shift : 控制照相机向正下方移动/控制玩家下潜（只有在己方颜色才有效）

鼠标控制

    鼠标移动视角

    鼠标左键发射子弹

    鼠标滚轮进行缩放

## 小组分工

刘一辰（3180102886）：地板渲染、涂色区域判定、光照渲染、实时阴影渲染、游戏音效

周义涵（3180105344）：油漆效果、粒子效果

黄海烽（3180102339）：物理引擎、游戏逻辑