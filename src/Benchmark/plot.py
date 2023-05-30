import math

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import colorsys


def plot_field(filename):
    data = pd.read_csv(filename)

    # Ignore Z for a 2d plot
    data.drop(columns=['z', 'fz'], inplace=True)

    # Convert to a 3d array, where the third axis contains [fx, fy]
    arr = data.set_index(['x', 'y']).to_xarray().to_array().transpose().to_numpy()

    # Determine the strongest force being shown
    max_magnitude = max([np.linalg.norm(xy) for xy in arr.reshape(-1, 2)])

    def to_rgb(xy):
        x, y = xy
        saturation = max(math.sqrt(np.linalg.norm(xy) / max_magnitude), 0.1)
        hue = (math.atan2(x, y) + math.pi) / (2 * math.pi)
        return np.asarray(colorsys.hsv_to_rgb(hue, saturation, 1.0))

    rgb = np.array([[to_rgb(xy) for xy in row] for row in arr])

    f, ax = plt.subplots()
    ax.imshow(
        rgb,
        extent=(data['x'].min(), data['x'].max(), data['y'].min(), data['y'].max())
    )

    plt.show()


def plot_sweep_n(filename):
    sweep_n = pd.read_csv(filename)
    print(sweep_n.to_string())

    sns.lineplot(data=sweep_n, x="n", y="Time (s)", hue="Solver")
    plt.show()


def plot_sweep_theta(filename):
    sweep_theta = pd.read_csv(filename)
    print(sweep_theta.to_string())

    sns.lineplot(data=sweep_theta, x="θ", y="Time (s)", hue="Solver")
    plt.show()


def plot_sweep_error(filename):
    sweep_theta = pd.read_csv(filename)
    print(sweep_theta)

    sns.lineplot(data=sweep_theta, x="% Error (Constitutional)", y="Time (s)", hue="Solver")
    plt.show()


def plot_interaction_counts(filename):
    interaction_counts = pd.read_csv(filename)

    # sns.histplot(data=interaction_counts, x="Passive Size", y="Active Size", bins=100)
    # plt.show()


def plot_interactions(filename):
    interaction_list = pd.read_csv(filename)

    node_node_interactions = interaction_list.loc[interaction_list['Type'] == "Node-Node"]
    print(node_node_interactions)

    sns.histplot(data=node_node_interactions, x="Passive Size", y="Active Size", bins=100)
    plt.show()


def main():
    # Customize the seaborn theme
    custom_params = {"axes.spines.right": False, "axes.spines.top": False}
    sns.set_theme(style="ticks", rc=custom_params)

    # plot_sweep_n('benchmarks/sweep-n.csv')
    # plot_sweep_theta('benchmarks/sweep-theta.csv')
    # plot_sweep_error('benchmarks/sweep-theta.csv')
    # plot_field('benchmarks/exact-field.csv')
    # plot_interactions('benchmarks/approximation-tracking.csv')
    plot_interaction_counts("benchmarks/interaction-counts/linear-bvh-exclusion-regions.csv")


if __name__ == "__main__":
    main()
