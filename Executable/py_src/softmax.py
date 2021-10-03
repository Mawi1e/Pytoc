import tensorflow.compat.v1 as tf
import matplotlib.pyplot as plt
tf.disable_v2_behavior()

# 2(A), 1(B), 0(C)
xData = [[1, 2, 1, 1], [2, 1, 3, 2], [3, 1, 3, 4], [4, 1, 5, 5], [1, 7, 5, 5], [1, 2, 5, 6], [1, 6, 6, 6], [1, 7, 7, 7]]
yData = [[0, 0, 1], [0, 0, 1], [0, 0, 1], [0, 1, 0], [0, 1, 0], [0, 1, 0], [1, 0, 0], [1, 0, 0]]

X = tf.placeholder(tf.float32, [None, 4])
Y = tf.placeholder(tf.float32, [None, 3])

W = tf.Variable(tf.random_normal([4, 3]), name = 'weight')
b = tf.Variable(tf.random_normal([3]), name = 'bias')

H = tf.nn.softmax(tf.matmul(X, W) + b)

# axis = 0 | 세로, axis = 1 | 가로
cost = tf.reduce_mean(-tf.reduce_sum(Y * tf.log(H), axis = 1))

train = tf.train.GradientDescentOptimizer(learning_rate = 1e-2).minimize(cost)

ina, inb, inc, ind = map(float,input().split())

with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())
    
    tryList = []
    costList = []
    
    for i in range(15001):
        sess.run(train, feed_dict = {X: xData, Y: yData})
        if i % 2000 == 0:
            print(i, sess.run(W), sess.run(b), sess.run(cost, feed_dict = {X: xData, Y: yData}))
            
        tryList.append(i)
        costList.append(sess.run(cost, feed_dict = {X: xData, Y: yData}))
    
    a = sess.run(H, feed_dict = {X: [[ina, inb, inc, ind]]})
    
    # axis = 0 | 세로, axis = 1 | 가로
    result = sess.run(tf.arg_max(a, 1))
    
    print(a, result)
    if result == 0:
        print('{}%로 C화분이라고 판단됨.'.format(float(float(a[0][result]) * 100.)))
    elif result == 1:
        print('{}%로 B화분이라고 판단됨.'.format(float(float(a[0][result]) * 100.)))
    elif result == 2:
        print('{}%로 A화분이라고 판단됨.'.format(float(float(a[0][result]) * 100.)))
    
    plt.plot(tryList, costList)
    plt.show()