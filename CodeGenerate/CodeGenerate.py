# 判断是否从主存读取
readFromMemory= -1
# 寄存器数组
regs = [6,7,8,9,10,11,12,13,14,15,16,17]
REG_NUMBER = len(regs)
# 变量名集合
tokens = set()
# 变量出现次数字典<name,number>
tokensNumber = {}
# 寄存器内容字典<reg,content>
regContent = {}
for reg in regs:
    regContent[reg] = "empty"
# 变量的存储位置字典
tokensAddr = {}


# 获取中间代码
def getMidCode():
    midcodeFile = open("midcode.txt",'r')
    midcodes = []
    lines = midcodeFile.readlines()
    for line in lines:
        codeValues = line.split()
        midcodes.append(codeValues)
        # 判断次数时无须考虑操作符
        for i in range(1,4):
            # 为空不做处理
            if(codeValues[i] == "NONE"):
                continue
            # 不空则对应次数加一
            else:
                if codeValues[i] in tokens:
                    tokensNumber[codeValues[i]] = tokensNumber[codeValues[i]] + 1
                else:
                    tokensNumber[codeValues[i]] = 1
                    tokens.add(codeValues[i])
    midcodeFile.close()
    return midcodes

# 把结果写入文件
def WriteToFile(midcode,argAddress1,argAddress2,resultAddress):
    targetFile = open("generateResult.txt","a+")
    op = midcode[0]
    arg1 = midcode[1]
    arg2 = midcode[2]
    result = midcode[3]
    if op == "-":
        opStr = "SUB"
    elif op == "+":
        opStr = "ADD"
    # 获取地址字符串
    argAddress1Str = str(arg1) if argAddress1 == readFromMemory else "$" + str(argAddress1)
    argAddress2Str = str(arg2) if argAddress2 == readFromMemory else "$" + str(argAddress2)
    resultAddressStr = str(result) if resultAddress == readFromMemory else "$" + str(resultAddress)
    # 写入文件
    targetFile.write(opStr+"\t"+argAddress1Str+","+argAddress2Str+","+resultAddressStr+"\n")
    targetFile.close()

# 获取结果x对应的地址
def getReg(midcode):
    arg1 = midcode[1]
    arg2 = midcode[2]
    result = midcode[3]
    global REG_NUMBER
    # 若arg1在寄存器中，且之后不会再次使用
    if(tokensAddr[arg1] in regs) and (tokensNumber[arg1] <= 1):
        resultAddress = tokensAddr[arg1]

    # 若变量arg2在寄存器中，且arg2之后不会再次使用
    if(tokensAddr[arg2] in regs) and (tokensNumber[arg2] <= 1):
        resultAddress = tokensAddr[arg2]
    # 若还有空闲寄存器
    elif REG_NUMBER > 0:
        for reg in regs:
            # 找到空闲值则将该值赋给resultAddress
            if regContent[reg] == "empty":
                REG_NUMBER = REG_NUMBER - 1
                regContent[reg] = result
                tokensAddr[result] = reg
                resultAddress = reg
                break
    # 否则，返回主存位置
    else:
        resultAddress = readFromMemory
    return resultAddress

if __name__ == "__main__":
    # 获取中间代码
    midcodes = getMidCode()
    # 默认从主存中访问
    for token in tokens:
        tokensAddr[token] = readFromMemory
    for midcode in midcodes:
        arg1 = midcode[1]
        arg2 = midcode[2]
        resultAddress = getReg(midcode)
        for i in range(3):
            # 将参数对应的使用次数减一
            x = midcode[i + 1]
            tokensNumber[x] = tokensNumber[x] - 1
        WriteToFile(midcode, tokensAddr[arg1], tokensAddr[arg2], resultAddress)