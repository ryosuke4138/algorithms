import argparse
import os
import time

from evaluate import main


def evaluate_all(data_path):
    sum = 0
    sum_time = 0
    for i in range(5):
        start_time = time.time()
        output_csv_file_name = 'error_analysis{}.csv'.format(i)
        dat = os.path.join(data_path, 'dat{}_'.format(i))
        hensyukyori = main(dat+'in', dat+'out', dat+'ref', is_analysis=True, output_csv_file_name=output_csv_file_name)
        sum += hensyukyori
        elapsed_time = time.time() - start_time
        sum_time += elapsed_time
        print(hensyukyori)
    print('[INFO] ave of hensyukyori: {}'.format(sum/5))
    print('[INFO] ave time: {}'.format(sum_time/5))


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-d', '--data_path', type=str, help="path where dat files are located")
    args = parser.parse_args()

    evaluate_all(args.data_path)
