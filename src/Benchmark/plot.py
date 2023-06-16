import math
import os.path

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import colorsys
from matplotlib import rcParams
from matplotlib.patches import Rectangle
import matplotlib.ticker as plticker

# Text-width in inches, to make sure everything fits in the latex document
text_width = 5

# Standard colors, to be used in all plots
brewer_colors = sns.color_palette("Set1", 5)
color_map = {
    'RBH': brewer_colors[0],
    'BH': brewer_colors[1],
    'LBVH': brewer_colors[4],
    'MVDR': brewer_colors[3],
    'FMM': brewer_colors[2],

    'Octree': 'blue',
    'Linear BVH': 'red',

    'Particle-Particle': 'gray',
    'Particle-Node': 'cadetblue',
    'Node-Particle': 'lightseagreen',
    'Node-Node': 'aquamarine'
}


def plot_field(filename, zoom_box=None):
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

    extent = (data['x'].min(), data['x'].max(), data['y'].min(), data['y'].max())

    rgb = np.array([[to_rgb(xy) for xy in row] for row in arr])

    plt.figure(figsize=(text_width / 2, text_width / 2))
    plt.imshow(
        rgb,
        origin='lower',
        extent=extent,
    )
    plt.gca().xaxis.set_major_locator(plticker.MaxNLocator(nbins=4, min_n_ticks=5))
    plt.gca().yaxis.set_major_locator(plticker.MaxNLocator(nbins=4, min_n_ticks=5))

    # Maybe we can omit X and Y labels?
    # plt.xlabel('X', size=9)
    # plt.ylabel('Y', size=9)

    # Mark the zoom box, if it's present
    if zoom_box is not None:
        (x_min, x_max, y_min, y_max) = zoom_box
        plt.gca().add_patch(Rectangle(
            (x_min, y_min),
            x_max - x_min, y_max - y_min,
            edgecolor='red',
            facecolor='none',
            lw=1
        ))

    plt.tight_layout()
    plt.savefig(os.path.splitext(filename)[0] + ".pdf")
    return extent


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
    df = pd.read_csv(filename)
    x_label = df.columns[0]
    particle_count = df['N'][0]
    print(particle_count)
    df = df[
        [x_label, 'Particle-Particle', 'Particle-Node', 'Node-Particle', 'Node-Node']
    ]
    df = df.loc[:, (df != 0).any(axis=0)]
    df.loc[:, df.columns != x_label] = \
        100 * df.loc[:, df.columns != x_label] / (particle_count * particle_count)
    print(df)

    ax = df.plot(kind='bar', stacked=True,
                 figsize=(text_width / 2, 3),
                 x=x_label, color=color_map)
    ax.set(xlabel=x_label, ylabel='Interactions (\% of Naive)')
    ax.set_xticklabels(df[x_label], rotation=45, ha='right', rotation_mode='anchor')

    plt.tight_layout()
    plt.legend(loc='lower left')
    plt.savefig(os.path.splitext(filename)[0] + "_vs_interactions.pdf")


def plot_times(filename):
    df = pd.read_csv(filename)
    x_label = df.columns[0]
    df = df[[x_label, 'Time']]
    print(df)

    ax = df.plot(kind='bar', legend=False,
                 figsize=(text_width / 2, 3),
                 x=x_label, color='gray')
    ax.set(xlabel=x_label, ylabel='Time (S / Iteration)')
    ax.set_xticklabels(df[x_label], rotation=45, ha='right', rotation_mode='anchor')

    plt.tight_layout()
    plt.savefig(os.path.splitext(filename)[0] + "_vs_time.pdf")


def plot_times_vs_n(filename):
    df = pd.read_csv(filename)
    x_label = df.columns[0]
    df = df[[x_label, 'N', 'Time']]
    print(df)

    df = df.drop(df[df[x_label] == 'RBH'].index)

    plt.figure(figsize=(text_width, 4))
    ax = sns.lineplot(
        data=df, x="N", y="Time", hue=x_label, palette=color_map
    )
    ax.set(ylabel='Time (S / Iteration)')

    plt.tight_layout()
    plt.savefig(os.path.splitext(filename)[0] + "_vs_time.pdf")
    plt.cla()


def plot_theta_vs_n(filename):
    df = pd.read_csv(filename)
    x_label = df.columns[0]
    df = df[[x_label, 'N', 'Theta']]
    print(df)

    df = df.drop(df[df['Solver'] == 'RBH'].index)

    ax = sns.lineplot(data=df, x="N", y="Theta", hue="Solver", palette=color_map)
    ax.set(ylabel='Theta (for 0.5\% Constitutional Error)')

    plt.tight_layout()
    plt.savefig(os.path.splitext(filename)[0] + "_vs_theta.pdf")
    plt.cla()


def plot_interaction_counts_vs_n(filename):
    df = pd.read_csv(filename)
    x_label = df.columns[0]
    df['Total Interactions'] = df['Particle-Particle'] + df['Particle-Node'] + df['Node-Particle'] + df['Node-Node']
    df = df[[x_label, 'N', 'Total Interactions']]
    print(df)

    df = df.drop(df[df['Solver'] == 'RBH'].index)

    ax = sns.lineplot(data=df, x="N", y="Total Interactions", hue="Solver", palette=color_map)
    ax.set(ylabel='Interaction Count')

    plt.tight_layout()
    plt.savefig(os.path.splitext(filename)[0] + "_vs_interaction-count.pdf")
    plt.cla()


def plot_interactions(filename):
    interaction_list = pd.read_csv(filename)

    node_node_interactions = interaction_list.loc[interaction_list['Type'] == "Node-Node"]
    print(node_node_interactions)

    sns.scatterplot(data=node_node_interactions, x="Passive Size", y="Active Size")
    plt.tight_layout()
    plt.savefig(os.path.splitext(filename)[0] + "_ratios.pdf")
    plt.cla()


def main():
    # Customize the seaborn theme
    custom_params = {"axes.spines.right": False, "axes.spines.top": False}
    sns.set_theme(style="ticks", rc=custom_params)

    plt.rc('text', usetex=True)
    plt.rc('font', family='times', size=11)
    plt.rc('axes', labelsize=11)
    plt.rc('xtick', labelsize=8)
    plt.rc('ytick', labelsize=8)
    plt.rc('legend', fontsize=9)

    # plot_sweep_n('benchmarks/sweep-n.csv')
    # plot_sweep_theta('benchmarks/sweep-theta.csv')
    # plot_sweep_error('benchmarks/sweep-theta.csv')
    # plot_interactions('benchmarks/approximation-tracking.csv')

    zoom_box = plot_field('benchmarks/sample-exact-1x1.csv')
    plot_field('benchmarks/sample-exact-8x8.csv', zoom_box)

    # plot_interaction_counts("benchmarks/linear-bvh-descent-criterion.csv")
    # plot_times("benchmarks/linear-bvh-descent-criterion.csv")

    plot_times_vs_n("benchmarks/all-solvers-random-data.csv")
    plot_theta_vs_n("benchmarks/all-solvers-random-data.csv")
    plot_interaction_counts_vs_n("benchmarks/all-solvers-random-data.csv")

    plot_times_vs_n("benchmarks/tree-construction-agora-data.csv")
    plot_times_vs_n("benchmarks/tree-construction-random-data.csv")


if __name__ == "__main__":
    main()
