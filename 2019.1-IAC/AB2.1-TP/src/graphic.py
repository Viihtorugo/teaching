import matplotlib.pyplot as plt

tempo = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
uso_ucp = [98, 102, 101, 103, 101, 101, 89.7, 90.1, 91.4, 92.3]
uso_mem = [102.4, 102.4, 102.4, 102.4, 102.4, 102.4, 102.4, 102.4, 102.4, 102.4]
uso_ucp_2nd = [102, 99.5, 101, 100, 102, 103, 103, 92.2, 93.5, 94.3]
uso_mem_2nd = [13791.23, 26173.44, 39485.44, 51716.10, 65163.27, 78159.88, 91156.48, 104194.05, 117194.75, 130211.84]

plt.plot(tempo, uso_ucp, color='g')

plt.ylim((50, 105))

plt.title("Uso intenso da UCP")
plt.xlabel("Tempo em segundos")
plt.ylabel("Uso da UCP (%)")

plt.legend(['UCP(%)'])

plt.savefig("./graphic/cpu(ucp).png")
plt.show()



plt.plot(tempo, uso_mem, color='m')
plt.ylim((40, 170))
plt.title('Uso intenso da UCP')
plt.xlabel('Tempo em segundos')
plt.ylabel('Uso da Memória (KB)')

plt.legend(['Memória(KB)'])

plt.savefig('./graphic/cpu(mem).png')
plt.show()



plt.plot(tempo, uso_ucp_2nd, color='g')
plt.ylim((50, 105))
plt.title('Uso intenso da UCP e Memória')
plt.xlabel('Tempo em segundos')
plt.ylabel('Uso da UCP (%)')

plt.legend(['UCP(%)'])

plt.savefig('./graphic/cpu-mem(ucp).png')
plt.show()



plt.plot(tempo, uso_mem_2nd, color='m')
plt.title('Uso intenso da UCP e Memória')
plt.xlabel('Tempo em segundos')
plt.ylabel('Uso da Memória (KB)')

plt.legend(['Memória(KB)'])

plt.savefig('./graphic/cpu-mem(mem).png')
plt.show()
