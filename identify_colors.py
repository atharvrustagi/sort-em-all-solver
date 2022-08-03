import numpy as np
from matplotlib import image, pyplot as plt


# (550, 50) (800, 700)

img = image.imread('screenshot.jpeg')
img1 = img[550:800, 50:700]
img2 = img[1000:1250, 50:700]
sz = 20
# increment of 70 in x direction
# increment of 122 in y direction
img1s = []
for i in range(0, img1.shape[0], 70):
    for j in range(0, img1.shape[1], 122):
        img1s.append(img1[i:i+sz, j:j+sz])
img2s = []
for i in range(0, img2.shape[0], 70):
    for j in range(0, 122*4, 122):
        img2s.append(img2[i:i+sz, j:j+sz])

colors = []

bottles = []

for p in img1s:
    clr = np.mean(p, axis=(0,1))
    diff = 100.0
    idx = -1
    for i, c in enumerate(colors):
        if np.sum((c-clr)**2) < diff:
            diff = np.sum((c-clr)**2)
            idx = i
    if idx == -1 or diff > 40:
        colors.append(clr)
        # print('size =', len(colors), 'diff =', diff)
        bottles.append(len(colors)-1)
    else:
        # print(idx)
        bottles.append(idx)
    
    # plt.imshow(p)
    # plt.show()

for p in img2s:
    clr = np.mean(p, axis=(0,1))
    diff = 100.0
    idx = -1
    for i, c in enumerate(colors):
        if np.sum((c-clr)**2) < diff:
            diff = np.sum((c-clr)**2)
            idx = i
    if idx == -1 or diff > 40:
        colors.append(clr)
        # print('size =', len(colors), 'diff =', diff)
        bottles.append(len(colors)-1)
    else:
        # print(idx)
        bottles.append(idx)
    
    # plt.imshow(p)
    # plt.show()

# print('size =', len(colors))
bottles = np.array(bottles, dtype = np.int8)
bottles1 = bottles[:24]
bottles2 = bottles[24:]

bottles1 = bottles1.reshape(4, 6).T
bottles2 = bottles2.reshape(4, 4).T
# print(bottles1)
# print(bottles2)
bottles = np.concatenate((bottles1, bottles2, np.zeros((2, 4), dtype=np.int8)-1), axis = 0)
# print(bottles)

for b in bottles:
    print('{ ', end='')
    for c in b[:-1]:
        print(c+1, end = ', ')
    print(b[-1]+1, end = ' },\n')
