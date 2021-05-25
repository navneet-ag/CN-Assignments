myfile=open('ques2b','r')
Lines=myfile.readlines()
Data=[]
for i in Lines:
	Data.append((i.split(" ")))
NumberOfPacketsReceived=int(Data[-2][3])

FinalData=Data[1:1+NumberOfPacketsReceived]
FinalTimeData=[]
for i in FinalData:
	FinalTimeData.append(i[-2])
for i in range(len(FinalTimeData)):
	FinalTimeData[i]=float(FinalTimeData[i][5:])
# for j in FinalTimeData:
# 	print(j)
FinalTimeData.sort()
# for j in FinalTimeData:
# 	print(j)
if(NumberOfPacketsReceived%2==0):
	print("Median latency is:"+str((FinalTimeData[NumberOfPacketsReceived//2 -1]+ FinalTimeData[NumberOfPacketsReceived//2])/2)+str(" ms"))
else:
	print("Median latency is:"+str(FinalTimeData[NumberOfPacketsReceived//2])+str(" ms"))
print("90 percentile latency :"+str(FinalTimeData[int(.9*NumberOfPacketsReceived) - 1])+str(" ms"))
print("99 percentile latency :"+str(FinalTimeData[int(.99*NumberOfPacketsReceived) - 1])+str(" ms"))
