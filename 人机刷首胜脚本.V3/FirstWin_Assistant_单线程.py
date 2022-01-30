import pyautogui
import time
import xlrd
import pyperclip
import win32api
import win32con
import asyncio


# asyncio.create_task() 函数用来并发运行作为 asyncio 任务 的多个协程。
# 定义键盘按键
def upGrade():
    # 升级
    keys = ["r", "e", "q", "w"]
    code = [82, 69, 81, 87]
    scancode = [0x13, 0x12, 0x10, 0x11]
    # ctrl
    win32api.keybd_event(17, 0x1D, 0, 0)  # q键位码是81 扫描码是10 16进制？
    for i in range(4):
        win32api.keybd_event(code[i], scancode[i], 0, 0)  # q键位码是81 扫描码是10 16进制？
        win32api.keybd_event(code[i], scancode[i], win32con.KEYEVENTF_KEYUP, 0)
    win32api.keybd_event(17, 0x1D, win32con.KEYEVENTF_KEYUP, 0)  # q键位码是81 扫描码是10 16进制？


def q_Down():
    # 按 q 键
    win32api.keybd_event(81, 0x10, 0, 0)  # q键位码是81 扫描码是10 16进制？
    win32api.keybd_event(81, 0x10, win32con.KEYEVENTF_KEYUP, 0)


def w_Down():
    # 按 w 键
    win32api.keybd_event(87, 0x11, 0, 0)
    win32api.keybd_event(87, 0x11, win32con.KEYEVENTF_KEYUP, 0)


def e_Down():
    # 按 e 键
    win32api.keybd_event(69, 0x12, 0, 0)
    win32api.keybd_event(69, 0x12, win32con.KEYEVENTF_KEYUP, 0)


def r_Down():
    # 按 r 键
    win32api.keybd_event(82, 0x13, 0, 0)
    win32api.keybd_event(82, 0x13, win32con.KEYEVENTF_KEYUP, 0)


def d_Down():
    win32api.keybd_event(68, 0x20, 0, 0)
    win32api.keybd_event(68, 0x20, win32con.KEYEVENTF_KEYUP, 0)


def f_Down():
    win32api.keybd_event(70, 0x21, 0, 0)
    win32api.keybd_event(70, 0x21, win32con.KEYEVENTF_KEYUP, 0)

def p_Down():
    win32api.keybd_event(80, 0x19, 0, 0)
    win32api.keybd_event(80, 0x19, win32con.KEYEVENTF_KEYUP, 0)
    #键码网址
    #https: // blog.csdn.net / weixin_42129248 / article / details / 89738210
    #扫描码网址 mark
    #https://blog.csdn.net/qq_37232329/article/details/79926440
def fight():
    for i in range(10):
        w_Down()
        time.sleep(2)
        e_Down()
        q_Down()
        time.sleep(1)
    f_Down()

# 打野怪流程
def killCreeps():
    # time.sleep(10)
    Creeps = ["蓝Buff", "蛤蟆", "三狼", "F4", "红Buff", "石头人"]
    CreepsPosition_X = [1717, 1691, 1718, 1774, 1786, 1798]
    CreepsPosition_Y = [929, 923, 954, 975, 997, 1023]
    # 怎么结束

    while True:
        lock_location = pyautogui.locateCenterOnScreen("inGame.png", confidence=0.9)
        if lock_location is not None:
            # if img == "lock.png":
            #     # 为什么要await?  阻塞？
            #     # await asyncio.sleep(10000000000)
            #     break
            for i in range(6):
                print("正在打{}".format(Creeps[i]))
                upGrade()
                pyautogui.moveTo(CreepsPosition_X[i], CreepsPosition_Y[i])
                time.sleep(0.2)
                pyautogui.mouseDown(button='right')
                time.sleep(0.1)
                pyautogui.mouseUp(button='right')
                fight()

        else:
            # print("不在游戏中")
            # 最多执行五次，所以设置了等待时间
            # await asyncio.sleep(15)  # time.sleep(150)
            time.sleep(0.5)
            break


# 定义鼠标事件

# pyautogui库其他用法 https://blog.csdn.net/qingfengxd1/article/details/108270159

def mouseClick(clickTimes, lOrR, img, reTry):
    if reTry == 1:
        while True:
            location =  pyautogui.locateCenterOnScreen(img, confidence=0.9)
            killCreeps()
            if location is not None:
                pyautogui.click(location.x, location.y, clicks=clickTimes, interval=0.2, duration=0.2, button=lOrR)
                break
            print("未找到匹配图片,1秒后重试")
            time.sleep(1)
            # await asyncio.sleep(1)
    elif reTry == -1:
        while True:
            location = pyautogui.locateCenterOnScreen(img, confidence=0.9)  # 90%相似？
            if location is not None:
                pyautogui.click(location.x, location.y, clicks=clickTimes, interval=0.2, duration=0.2, button=lOrR)
            time.sleep(1)
    elif reTry > 1:
        i = 1
        while i < reTry + 1:
            location = pyautogui.locateCenterOnScreen(img, confidence=0.9)
            if location is not None:
                pyautogui.click(location.x, location.y, clicks=clickTimes, interval=0.2, duration=0.2, button=lOrR)
                print("重复")
                i += 1
            time.sleep(1)
    elif reTry == -2:
        location = pyautogui.locateCenterOnScreen(img, confidence=0.9)  # 90%相似？
        killCreeps()
        if location is not None:
            pyautogui.click(location.x, location.y, clicks=clickTimes, interval=0.2, duration=0.2, button=lOrR)
        time.sleep(1)
        print("没找到" + img + "，执行下一个操作")


# 数据检查
# cmdType.value  1.0 左键单击    2.0 左键双击  3.0 右键单击  4.0 输入  5.0 等待  6.0 滚轮
# ctype     空：0
#           字符串：1
#           数字：2
#           日期：3
#           布尔：4
#           error：5
def dataCheck(sheet1):
    checkCmd = True
    # 行数检查
    if sheet1.nrows < 2:  # 只有一行
        print("没数据啊哥")
        checkCmd = False
    # 每行数据检查
    i = 1
    while i < sheet1.nrows:  # 当前行小于总行数
        # 第1列 操作类型检查
        cmdType = sheet1.row(i)[0]
        if cmdType.ctype != 2 or (cmdType.value != 1.0 and cmdType.value != 2.0 and cmdType.value != 3.0
                                  and cmdType.value != 4.0 and cmdType.value != 5.0 and cmdType.value != 6.0):
            print('第', i + 1, "行,第1列数据有毛病")
            checkCmd = False
        # 第2列 内容检查
        cmdValue = sheet1.row(i)[1]
        # 读图点击类型指令，内容必须为字符串类型
        if cmdType.value == 1.0 or cmdType.value == 2.0 or cmdType.value == 3.0:
            if cmdValue.ctype != 1:
                print('第', i + 1, "行,第2列数据有毛病")
                checkCmd = False
        # 输入类型，内容不能为空
        if cmdType.value == 4.0:
            if cmdValue.ctype == 0:
                print('第', i + 1, "行,第2列数据有毛病")
                checkCmd = False
        # 等待类型，内容必须为数字
        if cmdType.value == 5.0:
            if cmdValue.ctype != 2:
                print('第', i + 1, "行,第2列数据有毛病")
                checkCmd = False
        # 滚轮事件，内容必须为数字
        if cmdType.value == 6.0:
            if cmdValue.ctype != 2:
                print('第', i + 1, "行,第2列数据有毛病")
                checkCmd = False
        i += 1
    return checkCmd


# 任务     按顺序工作？对啊
def mainWork(img):
    i = 1
    while i < sheet1.nrows:
        # 取本行指令的操作类型
        cmdType = sheet1.row(i)[0]
        if cmdType.value == 1.0:
            # 取图片名称
            img = sheet1.row(i)[1].value
            reTry = 1
            if sheet1.row(i)[2].ctype == 2 and sheet1.row(i)[2].value != 0:
                reTry = sheet1.row(i)[2].value  # 获取第三行的重复次数
            print("单击左键", img)
            mouseClick(1, "left", img, reTry)
            # mouseClick(1,"right",img,reTry)
        # 2代表双击左键
        elif cmdType.value == 2.0:
            # 取图片名称
            img = sheet1.row(i)[1].value
            # 取重试次数
            reTry = 1
            if sheet1.row(i)[2].ctype == 2 and sheet1.row(i)[2].value != 0:
                reTry = sheet1.row(i)[2].value
            mouseClick(2, "left", img, reTry)
            print("双击左键", img)
        # 3代表右键
        elif cmdType.value == 3.0:
            # 取图片名称
            img = sheet1.row(i)[1].value
            # 取重试次数
            reTry = 1
            if sheet1.row(i)[2].ctype == 2 and sheet1.row(i)[2].value != 0:
                reTry = sheet1.row(i)[2].value
            mouseClick(1, "right", img, reTry)
            print("右键", img)
            # 4代表输入
        elif cmdType.value == 4.0:
            inputValue = sheet1.row(i)[1].value
            pyperclip.copy(inputValue)
            pyautogui.hotkey('ctrl', 'v')
            time.sleep(0.5)
            print("输入:", inputValue)
            # 5代表等待
        elif cmdType.value == 5.0:
            # 取图片名称
            waitTime = sheet1.row(i)[1].value
            time.sleep(waitTime)
            print("等待", waitTime, "秒")
        # 6代表滚轮
        elif cmdType.value == 6.0:
            # 取图片名称
            scroll = sheet1.row(i)[1].value
            pyautogui.scroll(int(scroll))
            print("滚轮滑动", int(scroll), "距离")
        i += 1


# 主程序
if __name__ == '__main__':
    file = 'cmd.xls'
    # 打开文件
    wb = xlrd.open_workbook(filename=file)
    # 通过索引获取表格sheet页
    sheet1 = wb.sheet_by_index(0)
    print('欢迎使用LOL人机模式打野RPA~')
    # 数据检查
    checkCmd = dataCheck(sheet1)
    if checkCmd:
        # key=input('选择功能: 1.只玩一把 2.一直玩 \n')
        # if key=='1':
        #     #循环拿出每一行指令
        #     mainWork(sheet1)

        # elif key=='2':
        while True:
            mainWork(sheet1)
            time.sleep(0.1)
            # print("等待0.1秒")
        # pass
    else:
        print('输入有误或者已经退出!')

    # loop = asyncio.get_event_loop()
    # tasks = []
    # tasks = [
    #     loop.create_task(killCreeps() ),
    #     loop.create_task(mainWork(sheet1)),
    # ]
    #
    # # loop.run_forever()
    # # killCreeps()5次， mainWork()1次
    # loop.run_until_complete(asyncio.wait(tasks))
    # loop.close()
    # async def main():
    #     task1 = asyncio.create_task(
    #         mainWork(sheet1))
    #
    #     task2 = asyncio.create_task(
    #        killCreeps())
    #
    #     print(f"started at {time.strftime('%X')}")
    #
    #     # Wait until both tasks are completed (should take
    #     # around 2 seconds.)
    #     await task1
    #     await task2
    #
    #     print(f"finished at {time.strftime('%X')}")
    #
    #
    # asyncio.run(main())
