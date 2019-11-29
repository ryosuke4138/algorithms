"""
evaluate the algorithm and output the csv file for analysis
"""
import argparse
import csv
import Levenshtein


def read_file(path):
    with open(path, 'r') as f:
        data = f.readline()
    return data


def get_index(string):
    return [i for i, c in enumerate(string) if c == 'x']


def eval(index, output_data, eval_data):
    # precision = sum(output_data[i] == eval_data[i] for i in index) / len(index)
    precision = Levenshtein.distance(output_data, eval_data)
    return precision


def analysis(index, input_data, output_data, eval_data):
    output = []
    for i in index:
        judge = (output_data[i] == eval_data[i])
        begin, end = max(0, i-10), i+11
        output.append([i, judge, output_data[i], eval_data[i],
                       input_data[begin:end], output_data[begin:end], eval_data[begin:end]])
    return output


def main(input_file, output_file, eval_file, is_analysis, output_csv_file_name):
    input_data = read_file(input_file)
    output_data = read_file(output_file)
    eval_data = read_file(eval_file)

    index = get_index(input_data)
    precision = eval(index, output_data, eval_data)
    print('precision: {}'.format(precision))

    if is_analysis:
        output = analysis(index, input_data, output_data, eval_data)
        with open(output_csv_file_name, 'w') as f:
            writer = csv.writer(f, lineterminator='\n')
            writer.writerow(output)
            print('correctly saved {}'.format(output_csv_file_name))


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input', type=str, help="data containing 'x'(T'), ex. dat1_in")
    parser.add_argument('-o', '--output', type=str, help='predicted data')
    parser.add_argument('-e', '--eval', type=str, help='validation data, ex. dat1_ref')
    parser.add_argument('--is_analysis', action='store_true', default=True)
    args = parser.parse_args()

    output_csv_file_name = 'error_analysis.csv'
    main(args.input, args.output, args.eval, args.is_analysis, output_csv_file_name=output_csv_file_name)
