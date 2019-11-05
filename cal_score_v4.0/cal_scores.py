import json
import numpy as np


def cal_score():

    with open("client.json") as fin:
        client = json.load(fin)

    sigma = float(client["mainConfig"]["timeOut"])

    with open("test.json") as fin:
        test_all = json.load(fin)

    DT_test_dict = {}
    test_time = {}
    test_id = 1
    for test in test_all:
        if test["callType"] == "send":
            DT_test_dict['"' + test["sysMessage"]["data"] + '"'] = [
                test_id, test["targetId"], test["sysMessage"]["target"], float(test["timestamp"])]
            test_time['"' + test["sysMessage"]["data"] + '"'] = float(test["timestamp"])
            test_id += 1

    min_timestamp = np.array(test_time.values()).min()
    max_timestamp = np.array(test_time.values()).max()

    with open('attach.log', "r") as fin:
        attach = fin.read().replace('\n', ',').split(',')

    DT_attach_dict = {}
    time_tao_original = {}
    for ii in range(len(attach)/3):
        if attach[3*ii] in DT_attach_dict.keys():
            if float(attach[3*ii+2]) < time_tao_original[attach[3*ii]]:
                DT_attach_dict[attach[3 * ii]] = float(attach[3 * ii + 1])
                time_tao_original[attach[3 * ii]] = float(attach[3 * ii + 2])
        else:
            DT_attach_dict[attach[3*ii]] = float(attach[3*ii+1])
            time_tao_original[attach[3*ii]] = float(attach[3*ii+2])

    test_num = len(DT_test_dict)
    lose_info = {}
    lose_count = 0
    for DT_test_key in DT_test_dict.keys():
        if DT_test_key in time_tao_original.keys():
            if time_tao_original[DT_test_key] > sigma:
                lose_info[DT_test_key] = DT_test_dict[DT_test_key]
                lose_count += 1
                del time_tao_original[DT_test_key]
        else:
            lose_info[DT_test_key] = DT_test_dict[DT_test_key]
            lose_count += 1

    tao_mean = np.mean(time_tao_original.values())
    tao_var = np.var(time_tao_original.values())
    eta = (test_num - lose_count) / float(test_num)
    score = ((sigma * 2 - tao_mean) / sigma * 3.5 + 0.5 / (tao_var + 1)) * (eta ** 2.5) * 10
    print "tao_mean: {:.5f} || tao_var: {:.5f} || eta: {:.5f} || recv/all: {}/{} || sigma: {:.5f}".format(
        tao_mean, tao_var, eta, test_num - lose_count, test_num, sigma)
    print "Your Score: {:.8f}".format(score)
    print "############################################## lose info ##############################################"

    lose_info_order_all = sorted(lose_info.items(), key=lambda x: x[1], reverse=False)
    for ii, lose_info_order in enumerate(lose_info_order_all):
        print ii+1, " msg_ind:{} || data:{} || {} --> {} || timestamp:{:.4f} || min_ts:{:.4f} || max_ts:{:.4f}".format(
            lose_info_order[1][0], lose_info_order[0], lose_info_order[1][1], lose_info_order[1][2], lose_info_order[1][3], min_timestamp, max_timestamp)


if __name__ == "__main__":
    cal_score()
