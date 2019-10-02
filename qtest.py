import sys
import qconfig
from math import *
n = int(sys.argv[1])
threshold = float(sys.argv[2])
delta = float(sys.argv[3])

shots = ceil((400/(1-threshold))*log(1/delta))


print('\nThreshold less than threshold_min.')
print('Entering Quantum circuit...\n')

from qiskit import *
from qiskit import IBMQ
from qiskit import BasicAer
from qiskit.aqua.components.oracles import TruthTableOracle
from qiskit.aqua.circuits.gates import mcmt,mct


IBMQ.save_account(qconfig.APItoken, overwrite=True)
IBMQ.load_accounts()

backend = IBMQ.get_backend('ibmq_qasm_simulator')


def swap(qc,q1,q2,anc):
    ln = range(len(q1))
    for i in ln:
        qc.cx(q1[i],q2[i])
        qc.cx(q2[i],q1[i])
        qc.cx(q1[i],q2[i])

s=''
dat_file = open('data.txt','r')
temp = dat_file.readline()
for i in range(2**n):
    temp = dat_file.readline()
    s+= list(temp.split("\t"))[1][:1]
dat_file.close()

print('Creating and Extracting oracle')
oracle = TruthTableOracle(s)
vr = oracle.variable_register
vr2 = QuantumRegister(len(vr))
anc = QuantumRegister(len(vr))
otr = oracle.output_register
qc1 = QuantumCircuit(vr,vr2,anc,otr)
qc1.h(vr)
qc1.x(otr)
qc1.h(otr)
qc1.barrier()

qc=oracle.circuit

qc2 = QuantumCircuit(vr,vr2,anc,otr)
qc2.h(vr)
swap(qc2,vr,vr2,anc)
qc2.h(vr)

qc3 = QuantumCircuit(vr,vr2,anc,otr)
swap(qc3,vr,vr2,anc)
qc3.h(vr2)
qc3.h(otr)
qc3.x(otr)

for i in range(len(vr)):
    qc3.cx(vr[i],vr2[i])

qc3.x(otr)
qc3.x(vr2)
qc3.mct(vr2,otr[0],anc)
qc3.x(vr2)

for i in range(len(vr)):
    qc3.cx(vr[i],vr2[i])

qc3.barrier()

print('Building circuit')
circ = qc1+qc+qc2+qc+qc3

c = ClassicalRegister(len(otr))
circ.add_register(c)
circ.measure(otr,c)

print('Running in backend')
job = execute(circ,backend,shots=shots)
print('Waiting for end of execution and for obtaining results')
result = job.result()
print('Aquired Results')
counts = job.result().get_counts()

print('counting ends')
if '1' in counts and counts['1']/counts['0']>0.5:
    est = counts['1']/shots
    #upper = (2**(n-1))*(1-sqrt(1-est))
    #lower = (2**(n-1))*(1-sqrt(threshold))
    upper = (1-sqrt(1-est))/2
    lower = (1-sqrt(threshold))/2
    print('')
    print('nl(f) belongs to [', round(lower,6), ',', round(upper,6), ']')
    print('\nTotal number of quantum queries made : ', shots)
