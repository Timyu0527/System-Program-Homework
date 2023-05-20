import objreader
import common

def processTRecord(line, csaddr, progaddr, memorycontent):
    addr = int("0x%s" % line[1:7], 16)
    addr += csaddr

    memoryArrayIndex = addr - progaddr
    memoryArrayIndex *= 2
    
    len = int("0x%s" % line[7:9], 16)
    
    for i in range(0, len*2):
        memorycontent[memoryArrayIndex] = line[9+i]
        memoryArrayIndex += 1

def processMRecord(line, csaddr, progaddr, memorycontent, ESTAB):
    # print(line[1:7])
    addr = int("0x%s" % line[1:7], 16)
    # print("progaddr: {}".format(progaddr))
    addr += csaddr
    # print("addr: {}".format(addr))

    memoryArrayIndex = addr - progaddr
    memoryArrayIndex *= 2
    # print("memoryArrayIndex: {}".format(memoryArrayIndex))
    
    ml = int("0x%s" % line[7:9], 16)
    
    if (ml == 5):
        memoryArrayIndex += 1
    # print("ml: {}".format(ml))
        
    currentMemoryValue = "".join(memorycontent)[memoryArrayIndex:memoryArrayIndex+ml]
    # print("currentMemoryValue: {}".format(currentMemoryValue))
    # print(current)
    
    value = common.getDec("0x%s" % currentMemoryValue, ml*4)
    # print("b value: {}".format(value))
    token = line[10:len(line)-1]
    # print("token: {} {}".format(token, ESTAB[token]))
    if line[9] == '+':
        value += ESTAB[token]
    else:
        value -= ESTAB[token]
    
    # print("a value: {}".format(value))
    result = common.hexToString(common.getHex(value, ml*4), ml)
    # print(common.getHex(value, ml*4))
    # print("result: {}".format(result))
    
    for i in range(0, ml):
        memorycontent[memoryArrayIndex] = result[i]
        memoryArrayIndex += 1

def execute(argv, ESTAB, memorycontent):
    PROGADDR = int ("%s" % argv[1], 16)
    # print(PROGADDR)
    CSADDR = PROGADDR
    # print(CSADDR)
        
    for i in range(2, len(argv)):
        lines = objreader.readfile(argv[i])
        # Header Record
        csname = objreader.getNameWithoutSpace(lines[0][1:7])
        cslength = int("%s" % lines[0][13:19], 16)
        
        for l in range(1, len(lines)):
            if lines[l][0:1] == 'T':
                processTRecord(lines[l], CSADDR, PROGADDR, memorycontent)
            if lines[l][0:1] == 'M':
                processMRecord(lines[l], CSADDR, PROGADDR, memorycontent, ESTAB)
                
        CSADDR += cslength

    # for i in range(0, len(memorycontent)):
    #     print(memorycontent[i], end ="")
    #     if (i%32 == 31):
    #         print()
    # print(memorycontent)
