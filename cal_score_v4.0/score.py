import json

def deal_server_msg(path):
    server = json.load(open(path + 'server.json'))
    return server['mainConfig']['timeOut']

def deal_test_msg(path):
    test = json.load(open(path + 'test.json'))
    return len(test) / 2

def deal_attach_msg(path):
    result = {}
    with open(path + 'attach1.log') as fattach:
        for line in fattach.readlines():
            h, t, d = line.split(',')
            if h not in result:
                result[h] = float(d)
    return result

def main():
    N = input()
    path = 'input%d/' % N
    Q = deal_test_msg(path)
    T = deal_server_msg(path)
    attach = deal_attach_msg(path)
    
    attach = [i for i in attach.iteritems() if i[1] <= T]
    eta = len(attach) * 1.0 / Q
    tao = [i[1] for i in attach]
    tao_avg = sum(tao) / len(tao)
    D = sum([(i - tao_avg) ** 2 for i in tao]) / len(tao)
    print {'sigma': T, 'avg': tao_avg, 'D': D, 'eta': eta} 
    score = ((T * 2 - tao_avg) / T * 3.5 + 0.5 / (D + 1)) * eta ** 2.5 * 10
    print score


if __name__ == '__main__':
    main()

