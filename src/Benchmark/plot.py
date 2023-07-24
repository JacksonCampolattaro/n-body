import math
import os.path

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib as mpl
import seaborn
import seaborn as sns
import colorsys
from matplotlib import rcParams
from matplotlib.patches import Rectangle
import matplotlib.ticker as plticker
from mpl_toolkits.mplot3d import Axes3D

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
error_color_map = 'magma'
directional_color_map = mpl.colors.LinearSegmentedColormap.from_list(
    "directional",
    sns.color_palette("husl", 4) + [sns.color_palette("husl", 4)[0]]
)


def polar_to_rgb(r, th):
    r = np.cbrt(np.clip(r, 0.0, 1.0))
    return np.asarray(directional_color_map(th / (2 * np.pi), r))


def field_key():

    fig = plt.figure(figsize=(0.5*text_width, 0.5*text_width))

    rad = np.geomspace(0.00001, 100, 100)
    azm = np.linspace(2 * np.pi / 100, 2 * np.pi, 100)
    r, th = np.meshgrid(rad, azm)
    colors = np.vectorize(polar_to_rgb, signature='(),()->(n)')(r / 100, th)

    plt.subplot(projection="polar")
    plt.pcolormesh(th, r, colors, shading='nearest')
    plt.plot(azm, r, color='k', ls='none')
    plt.grid()
    plt.gca().set_axisbelow(False)
    plt.gca().set_rscale('symlog')
    plt.gca().yaxis.set_major_formatter(mpl.ticker.PercentFormatter())

    plt.tight_layout()
    plt.savefig("benchmarks/field_key.pdf")
    plt.clf()


def field_error_key():
    fig = plt.figure(figsize=(text_width, text_width / 6))
    mpl.colorbar.ColorbarBase(fig.gca(), orientation='horizontal',
                              cmap=error_color_map,
                              norm=mpl.colors.Normalize(0, 0.5),  # vmax and vmin
                              extend='max',
                              format=mpl.ticker.PercentFormatter()
                              )
    plt.tight_layout()
    plt.savefig("benchmarks/error_key.pdf")
    plt.clf()


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
        r = np.log(np.linalg.norm(xy) + 1.0) / np.log(max_magnitude + 1.0)
        th = (np.arctan2(-x, -y) + math.pi)
        if r > 1.0:
            return np.asarray([0.0, 0.0, 0.0, 1.0])
        return polar_to_rgb(r, th)

    extent = (data['x'].min(), data['x'].max(), data['y'].min(), data['y'].max())

    rgb = np.array([[to_rgb(xy) for xy in row] for row in arr])

    plt.figure(figsize=(text_width / 2, text_width / 2))
    plt.imshow(
        rgb,
        origin='lower',
        extent=extent,
        interpolation='nearest'
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
    plt.clf()
    plt.cla()
    return extent


def plot_field_error(filename):
    data = pd.read_csv(filename)

    # Ignore Z for a 2d plot
    data.drop(columns=['z'], inplace=True)

    # Here, we only care about error magnitudes
    data.drop(columns=['ex', 'ey', 'ez'], inplace=True)

    # Convert to a 3d array, where the third axis contains [e]
    err = data.set_index(['x', 'y']).to_xarray().to_array().transpose().to_numpy()
    err = np.squeeze(err)

    # Convert the error to a percentage
    err = err * 100

    extent = (data['x'].min(), data['x'].max(), data['y'].min(), data['y'].max())

    plt.figure(figsize=(text_width / 2, text_width / 2))
    image = plt.imshow(
        err,
        vmin=0.0,
        vmax=0.5,
        cmap=error_color_map,
        origin='lower',
        extent=extent,
    )
    plt.gca().xaxis.set_major_locator(plticker.MaxNLocator(nbins=4, min_n_ticks=5))
    plt.gca().yaxis.set_major_locator(plticker.MaxNLocator(nbins=4, min_n_ticks=5))

    # Maybe we can omit X and Y labels?
    # plt.xlabel('X', size=9)
    # plt.ylabel('Y', size=9)

    plt.tight_layout()
    plt.savefig(os.path.splitext(filename)[0] + ".pdf")
    plt.clf()
    return extent


def plot_sweep_n(filename):
    sweep_n = pd.read_csv(filename)

    sns.lineplot(data=sweep_n, x="n", y="Time (s)", hue="Solver")
    plt.show()


def plot_sweep_theta(filename):
    sweep_theta = pd.read_csv(filename)

    sns.lineplot(data=sweep_theta, x="θ", y="Time (s)", hue="Solver")
    plt.show()


def plot_sweep_error(filename):
    sweep_theta = pd.read_csv(filename)

    sns.lineplot(data=sweep_theta, x="% Error (Constitutional)", y="Time (s)", hue="Solver")
    plt.show()


def plot_interaction_counts(filename):
    df = pd.read_csv(filename)
    x_label = df.columns[0]
    particle_count = df['N'][0]
    df = df[
        [x_label, 'Particle-Particle', 'Particle-Node', 'Node-Particle', 'Node-Node']
    ]
    df = df.loc[:, (df != 0).any(axis=0)]
    df.loc[:, df.columns != x_label] = \
        100 * df.loc[:, df.columns != x_label] / (particle_count * particle_count)

    ax = df.plot(kind='bar', stacked=True,
                 figsize=(text_width / 2, 3),
                 x=x_label, color=color_map)
    ax.set(xlabel=x_label, ylabel='Interactions (\% of Naive)')
    ax.set_xticklabels(df[x_label], rotation=45, ha='right', rotation_mode='anchor')

    plt.tight_layout()
    plt.legend(loc='lower left')
    plt.savefig(os.path.splitext(filename)[0] + "_vs_interactions.pdf")
    plt.clf()


def plot_times(filename):
    df = pd.read_csv(filename)
    x_label = df.columns[0]
    df = df[[x_label, 'Time']]

    ax = df.plot(kind='bar', legend=False,
                 figsize=(text_width / 2, 3),
                 x=x_label, color='gray')
    ax.set(xlabel=x_label, ylabel='Time (S / Iteration)')
    ax.set_xticklabels(df[x_label], rotation=45, ha='right', rotation_mode='anchor')

    plt.tight_layout()
    plt.savefig(os.path.splitext(filename)[0] + "_vs_time.pdf")
    plt.clf()


def plot_times_vs_n(filename):
    df = pd.read_csv(filename)
    x_label = df.columns[0]
    df = df[[x_label, 'N', 'Time']]

    df = df.drop(df[df[x_label] == 'RBH'].index)

    plt.figure(figsize=(text_width, 4))
    ax = sns.lineplot(
        data=df, x="N", y="Time", hue=x_label, palette=color_map
    )
    ax.set(ylabel='Time (S / Iteration)')

    plt.tight_layout()
    plt.savefig(os.path.splitext(filename)[0] + "_vs_time.pdf")
    plt.clf()


def plot_theta_vs_n(filename):
    df = pd.read_csv(filename)
    x_label = df.columns[0]
    df = df[[x_label, 'N', 'Theta']]

    df = df.drop(df[df['Solver'] == 'RBH'].index)

    ax = sns.lineplot(data=df, x="N", y="Theta", hue="Solver", palette=color_map)
    ax.set(ylabel='Theta (for 0.5\% Constitutional Error)')

    plt.tight_layout()
    plt.savefig(os.path.splitext(filename)[0] + "_vs_theta.pdf")
    plt.clf()


def plot_interaction_counts_vs_n(filename):
    df = pd.read_csv(filename)
    x_label = df.columns[0]
    df['Total Interactions'] = df['Particle-Particle'] + df['Particle-Node'] + df['Node-Particle'] + df['Node-Node']
    df = df[[x_label, 'N', 'Total Interactions']]

    df = df.drop(df[df['Solver'] == 'RBH'].index)

    ax = sns.lineplot(data=df, x="N", y="Total Interactions", hue="Solver", palette=color_map)
    ax.set(ylabel='Interaction Count')

    plt.tight_layout()
    plt.savefig(os.path.splitext(filename)[0] + "_vs_interaction-count.pdf")
    plt.clf()


def plot_interactions(filename):
    interaction_list = pd.read_csv(filename)

    node_node_interactions = interaction_list.loc[interaction_list['Type'] == "Node-Node"]

    sns.scatterplot(data=node_node_interactions, x="Passive Size", y="Active Size")
    plt.tight_layout()
    plt.savefig(os.path.splitext(filename)[0] + "_ratios.pdf")
    plt.clf()


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

    # plot_interaction_counts("benchmarks/linear-bvh-descent-criterion.csv")
    # plot_times("benchmarks/linear-bvh-descent-criterion.csv")

    # print("Generating keys for field plots")
    # field_key()
    # field_error_key()
    #
    # print("Plotting exact fields")
    # zoom_box = plot_field('benchmarks/sample-exact-zoom.csv')
    # plot_field('benchmarks/sample-exact.csv', zoom_box)
    #
    # print("Plotting multipole moment approximations")
    # plot_field('benchmarks/sample-monopole-moment.csv')
    # plot_field_error('benchmarks/sample-monopole-moment-error.csv')
    # plot_field_error('benchmarks/sample-monopole-moment-extended-error.csv')
    # plot_field('benchmarks/sample-net-quadrupole-moment.csv')
    # plot_field('benchmarks/sample-quadrupole-moment.csv')
    # plot_field_error('benchmarks/sample-quadrupole-moment-error.csv')
    # plot_field('benchmarks/sample-octupole-moment.csv')
    # plot_field_error('benchmarks/sample-octupole-moment-error.csv')
    # plot_field('benchmarks/sample-hexadecupole-moment.csv')
    # plot_field_error('benchmarks/sample-hexadecupole-moment-error.csv')
    #
    # print("Plotting multipole field approximations")
    # plot_field('benchmarks/sample-vector-field.csv')
    # plot_field_error('benchmarks/sample-vector-field-error.csv')
    # plot_field('benchmarks/sample-net-quadrupole-field.csv')
    # plot_field('benchmarks/sample-quadrupole-field.csv')
    # plot_field_error('benchmarks/sample-quadrupole-field-error.csv')
    # plot_field_error('benchmarks/sample-quadrupole-field-zoom-error.csv')
    # plot_field('benchmarks/sample-octupole-field.csv')
    # plot_field_error('benchmarks/sample-octupole-field-error.csv')
    # plot_field('benchmarks/sample-hexadecupole-field.csv')
    # plot_field_error('benchmarks/sample-hexadecupole-field-error.csv')
    # plot_field('benchmarks/sample-triacontadyupole-field.csv')
    # plot_field_error('benchmarks/sample-triacontadyupole-field-error.csv')

    print("Plotting tree fields")
    plot_field('benchmarks/sample-vector-tree.csv')
    plot_field('benchmarks/sample-quadrupole-tree.csv')
    plot_field('benchmarks/sample-octupole-tree.csv')
    plot_field('benchmarks/sample-hexadecupole-tree.csv')

    # print("Plotting solver benchmarks")
    # plot_times_vs_n("benchmarks/all-solvers-random-data.csv")
    # plot_theta_vs_n("benchmarks/all-solvers-random-data.csv")
    # plot_interaction_counts_vs_n("benchmarks/all-solvers-random-data.csv")
    #
    # print("Plotting tree benchmarks")
    # plot_times_vs_n("benchmarks/tree-construction-agora-data.csv")
    # plot_times_vs_n("benchmarks/tree-construction-random-data.csv")


if __name__ == "__main__":
    main()
