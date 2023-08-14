import math
import os.path
import glob

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
    'BH (Quadrupole)': sns.light_palette(brewer_colors[1], n_colors=4)[1],
    'BH (Octupole)': sns.light_palette(brewer_colors[1], n_colors=4)[2],
    'BH (Hexadecupole)': sns.light_palette(brewer_colors[1], n_colors=4)[3],

    'LBVH': brewer_colors[4],
    'LBVH (Quadrupole)': sns.light_palette(brewer_colors[4], n_colors=4)[1],
    'LBVH (Octupole)': sns.light_palette(brewer_colors[4], n_colors=4)[2],
    'LBVH (Hexadecupole)': sns.light_palette(brewer_colors[4], n_colors=4)[3],

    'MVDR': brewer_colors[3],
    'MVDR (Quadrupole)': sns.light_palette(brewer_colors[3], n_colors=4)[1],
    'MVDR (Octupole)': sns.light_palette(brewer_colors[3], n_colors=4)[2],
    'MVDR (Hexadecupole)': sns.light_palette(brewer_colors[3], n_colors=4)[3],

    'FMM': brewer_colors[2],
    'FMM (Quadrupole)': sns.light_palette(brewer_colors[2], n_colors=4)[1],
    'FMM (Octupole)': sns.light_palette(brewer_colors[2], n_colors=4)[2],
    'FMM (Hexadecupole)': sns.light_palette(brewer_colors[2], n_colors=4)[3],

    'Octree': brewer_colors[1],
    'Linear BVH': brewer_colors[0],

    'Time': brewer_colors[1],
    'Error': brewer_colors[0],

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
histogram_color_map = sns.cubehelix_palette(start=.5, rot=-.75, as_cmap=True)


def polar_to_rgb(r, th):
    r = np.cbrt(np.clip(r, 0.0, 1.0))
    return np.asarray(directional_color_map(th / (2 * np.pi), r))


def field_key():
    fig = plt.figure(figsize=(0.5 * text_width, 0.5 * text_width))

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


def plot_solvers_vs_n_vs_time(filename):
    sweep_n = pd.read_csv(filename)

    sns.lineplot(data=sweep_n, x="n", y="Time (s)", hue="Solver")
    plt.show()


def plot_solvers_vs_theta_vs_time(filename):
    sweep_theta = pd.read_csv(filename)

    sns.lineplot(data=sweep_theta, x="θ", y="Time (s)", hue="Solver")
    plt.show()


def plot_theta_vs_time_and_error(filename, multipole):
    df = pd.read_csv(filename)
    df = df[df["Multipole Order"] == multipole]
    fig = plt.figure(figsize=(text_width, text_width / 2))

    with plt.rc_context({
        "axes.spines.right": False,
        "axes.spines.left": True,
        "ytick.color": color_map["Time"]
    }):
        ax1 = sns.lineplot(data=df, x="θ", y="Time (s)", color=color_map["Time"])
        ax1.set(xlabel="$\\theta$", ylabel='Time (s)')
        # ax1.set_yscale('log')
        ax1.tick_params(axis='y', colors=color_map["Time"])
        ax1.spines['left'].set_color(color_map["Time"])

    with plt.rc_context({
        "axes.spines.right": True,
        "axes.spines.left": False,
        "ytick.color": color_map["Error"]
    }):
        ax2 = plt.twinx()
        sns.lineplot(ax=ax2, data=df, x="θ", y="% Error (Constitutional)", color=color_map["Error"])
        # ax2.set_yscale('log')
        ax2.set(xlabel="$\\theta$", ylabel='\\% Error (Constitutional)')
        ax2.spines['right'].set_color(color_map["Error"])
        x_range = ax2.get_xlim()
        sns.lineplot(ax=ax2, x=x_range, y=[0.5, 0.5], color=color_map["Error"], alpha=0.25)
        sns.lineplot(ax=ax2, x=x_range, y=[1.0, 1.0], color=color_map["Error"], alpha=0.25)

    plt.tight_layout()
    plt.savefig(os.path.splitext(filename)[0] + "-vs-time-and-error.pdf")
    plt.clf()


def plot_solvers_vs_error_vs_time(filename):
    fig = plt.figure(figsize=(text_width, text_width))
    sweep_theta = pd.read_csv(filename)

    ax = sns.lineplot(data=sweep_theta, x="% Error (Constitutional)", y="Time (s)", hue="Solver")
    ax.set(xlabel="\\% Error (Constitutional)")
    # ax.set_xscale('log')
    # ax.set_yscale('log')
    y_range = ax.get_ylim()
    sns.lineplot(x=[0.5, 0.5], y=y_range, ax=ax, color="black")
    sns.lineplot(x=[1.0, 1.0], y=y_range, ax=ax, color="black")
    plt.savefig(os.path.splitext(filename)[0] + "-error-vs-time.pdf")

    plt.tight_layout()
    plt.clf()


def plot_error_vs_time(filename, multipole_order):
    fig = plt.figure(figsize=(text_width, text_width / 2))
    df = pd.read_csv(filename)
    base_color = color_map[df["Solver"][0]]
    df = df[df["Multipole Order"] == multipole_order]

    ax = sns.lineplot(
        data=df,
        x="% Error (Constitutional)", y="Time (s)",
        color=base_color
    )
    ax.set(xlabel="\\% Error (Constitutional)")
    ax.set_xscale('log')
    ax.set_yscale('log')
    # ax.set_xlim(0.1, 10)
    y_range = ax.get_ylim()
    sns.lineplot(x=[0.5, 0.5], y=y_range, ax=ax, color="black")
    sns.lineplot(x=[1.0, 1.0], y=y_range, ax=ax, color="black")

    plt.tight_layout()
    plt.savefig(os.path.splitext(filename)[0] + f"-{multipole_order.lower()}-error-vs-time.pdf")
    plt.clf()


def plot_multipoles_vs_error_vs_time(filename):
    fig = plt.figure(figsize=(0.5 * text_width, 0.5 * text_width))
    df = pd.read_csv(filename)
    base_color = color_map[df["Solver"][0]]

    ax = sns.lineplot(
        data=df,
        x="% Error (Constitutional)", y="Time (s)",
        hue="Multipole Order", palette=sns.light_palette(base_color, n_colors=4)[1:]
    )
    ax.set(xlabel="\\% Error (Constitutional)")
    ax.legend(loc="upper right")
    ax.set_xscale('log')
    ax.set_yscale('log')
    ax.set_xlim(0.1, 5)
    ax.set_ylim(0.1, 15)
    #sns.move_legend(ax, "upper right")
    y_range = ax.get_ylim()
    sns.lineplot(x=[0.5, 0.5], y=y_range, ax=ax, color="black")
    sns.lineplot(x=[1.0, 1.0], y=y_range, ax=ax, color="black")

    plt.tight_layout()
    plt.savefig(os.path.splitext(filename)[0] + "-error-vs-time.pdf")
    plt.clf()


def plot_interaction_counts(filename):
    df = pd.read_csv(filename)
    df.style.format('{:,}')
    print(df.to_latex(
        columns=['Solver', 'Particle-Particle', 'Particle-Node', 'Node-Particle', 'Node-Node', 'Approximation Ratio'],
        float_format="%.4f",
        index=False
    ))

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
                 x=x_label,
                 color=color_map, linewidth=0)
    ax.set(xlabel=x_label, ylabel='Interactions (\% of Naive)')
    ax.set_xticklabels(df[x_label], rotation=45, ha='right', rotation_mode='anchor')

    plt.tight_layout()
    plt.legend(loc='lower left')
    plt.savefig(os.path.splitext(filename)[0] + "_vs_interactions.pdf")
    plt.clf()


def plot_interaction_distances_vs_sizes(filename, theta=None):
    df = pd.read_csv(filename)
    df['Max Size'] = df[['Active Size', 'Passive Size']].max(axis=1)
    df = df[['Distance', 'Max Size']]
    print(df)

    min_x = df['Distance'].min()
    max_x = df['Distance'].max()
    min_y = df['Max Size'].min()
    max_y = df['Max Size'].max()

    plt.figure(figsize=(text_width / 2, text_width / 2))
    ax = sns.histplot(
        data=df, x='Distance', y='Max Size',
        bins=(np.geomspace(min_x, max_x, num=100), np.geomspace(min_y, max_y, num=64)),
        cmap=histogram_color_map,
        linewidth=-0.1
        # marker="o", color=(*brewer_colors[1], 0.25),
    )
    ax.set_xscale('log', base=2)
    ax.set_yscale('log', base=2)
    ax.set(ylabel='Node Size')

    if theta is not None:
        sns.lineplot(
            x=[min_x, max_x], y=[min_x * theta, max_x * theta], ax=ax,
            color="black"
        )

    plt.tight_layout()
    plt.savefig(os.path.splitext(filename)[0] + "-distance-vs-size.pdf")
    plt.clf()


def plot_dual_interaction_distances_vs_ratios(filename):
    df = pd.read_csv(filename)
    df = df.loc[df["Type"] == 'Node-Node']
    df['Ratio'] = df['Active Size'] / df['Passive Size']
    df = df[['Distance', 'Ratio']]
    print(df)

    min_x = df['Distance'].min()
    max_x = df['Distance'].max()
    min_y = df['Ratio'].min()
    max_y = df['Ratio'].max()

    plt.figure(figsize=(text_width / 2, text_width / 2))
    ax = sns.histplot(
        data=df, x='Distance', y='Ratio',
        bins=(np.geomspace(min_x, max_x, num=64), np.geomspace(min_x, max_x, num=64)),
        cmap=histogram_color_map,
        linewidth=-0.1
        # marker="o", color=(*brewer_colors[1], 0.25),
    )
    ax.set_xscale('log', base=2)
    ax.set_yscale('log', base=2)
    ax.set(ylabel='Active Size / Passive Size')
    # ax.set_aspect('equal', adjustable='box')

    # sns.lineplot(
    #     x=[min_x, max_x], y=[min_x, max_x], ax=ax,
    #     color="black", alpha=0.3
    # )

    plt.tight_layout()
    plt.savefig(os.path.splitext(filename)[0] + "-distance-vs-ratio.pdf")
    plt.clf()


def plot_dual_interaction_distances_vs_sizes(filename, theta=None):
    df = pd.read_csv(filename)
    df = df[df["Type"] == "Node-Node"]
    df['Max Size'] = df[['Active Size', 'Passive Size']].max(axis=1)
    print(df)

    min_x = df['Distance'].min()
    max_x = df['Distance'].max()
    min_y = df['Max Size'].min()
    max_y = df['Max Size'].max()

    plt.figure(figsize=(text_width / 2, text_width / 2))
    ax = sns.histplot(
        data=df, x='Distance', y='Max Size',
        bins=(np.geomspace(min_x, max_x, num=100), np.geomspace(min_y, max_y, num=64)),
        cmap=histogram_color_map,
        linewidth=-0.1
    )
    ax.set_xscale('log', base=2)
    ax.set_yscale('log', base=2)
    ax.set(ylabel='Node Size')

    if theta is not None:
        sns.lineplot(
            x=[min_x, max_x], y=[min_x * theta, max_x * theta], ax=ax,
            color="black"
        )

    plt.tight_layout()
    plt.savefig(os.path.splitext(filename)[0] + "-distance-vs-max-size.pdf")
    plt.clf()


def plot_dual_interaction_sizes(filename):
    df = pd.read_csv(filename)
    df = df.loc[df["Type"] == 'Node-Node']
    print(df)

    min_x = df['Passive Size'].min()
    max_x = df['Passive Size'].max()

    plt.figure(figsize=(text_width / 2, text_width / 2))
    ax = sns.histplot(
        data=df, x='Passive Size', y='Active Size',
        bins=(np.geomspace(min_x, max_x, num=64), np.geomspace(min_x, max_x, num=64)),
        cmap=histogram_color_map,
        linewidth=-0.1
        # marker="o", color=(*brewer_colors[1], 0.25),
    )
    ax.set_xscale('log', base=2)
    ax.set_yscale('log', base=2)
    # ax.set(ylabel='Active Size / Passive Size')
    ax.set_aspect('equal', adjustable='box')

    sns.lineplot(
        x=[min_x, max_x], y=[min_x * 2, max_x * 2], ax=ax,
        color="black", alpha=0.3
    )

    plt.tight_layout()
    plt.savefig(os.path.splitext(filename)[0] + "-sizes.pdf")
    plt.clf()


def plot_tree_sizes(filename):
    df = pd.read_csv(filename)
    df['Tree Size (bytes)'] = df["Node Count"] * 48

    plt.figure(figsize=(text_width / 2, text_width / 2))
    ax = sns.lineplot(
        data=df, x="N", y="Tree Size (bytes)", hue="Tree", palette=color_map
    )

    plt.tight_layout()
    plt.savefig(os.path.splitext(filename)[0] + "-vs-size.pdf")
    plt.clf()


def solver_median_values_to_latex(filename):
    df = pd.read_csv(filename)
    df["Solver"] = df["Solver"] + " (" + df["Multipole Order"] + ")"
    df.groupby("Solver")[df.select_dtypes(include=np.number).columns.tolist()].median()
    df = df.pivot_table(index=['Solver'], columns=['N'], values='Time')
    print(df.to_latex(
        float_format="%.2f",
    ))


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


def plot_times_vs_n(filename, multipole_order=None):
    df = pd.read_csv(filename)
    x_label = df.columns[0]

    if multipole_order is not None:
        df = df[df['Multipole Order'] == multipole_order]

    df["Solver"] = df["Solver"] + " (" + df["Multipole Order"] + ")"

    plt.figure(figsize=(text_width, 4))
    ax = sns.lineplot(
        data=df, x="N", y="Time", hue=x_label, palette=color_map
    )
    ax.set(ylabel='Time (S / Iteration)')
    #ax.set_xscale('log')
    #ax.set_yscale('log')

    plt.tight_layout()
    plt.savefig(os.path.splitext(filename)[0] + "-vs-time.pdf")
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


def plot_interaction_counts_vs_n(filename, solver=None):
    df = pd.read_csv(filename)
    x_label = df.columns[0]

    df['Total Interactions'] = df['Particle-Particle'] + df['Particle-Node'] + df['Node-Particle'] + df['Node-Node']
    df["Solver"] = df["Solver"] + " (" + df["Multipole Order"] + ")"
    df = df[[x_label, 'N', 'Total Interactions']]

    plt.figure(figsize=(text_width, text_width / 2))
    ax = sns.lineplot(data=df, x="N", y="Total Interactions", hue="Solver", palette=color_map)
    ax.set(ylabel='Interaction Count')

    plt.tight_layout()
    plt.savefig(os.path.splitext(filename)[0] + "-n-vs-interactions.pdf")
    plt.clf()


def plot_interactions(filename):
    interaction_list = pd.read_csv(filename)

    node_node_interactions = interaction_list.loc[interaction_list['Type'] == "Node-Node"]

    sns.scatterplot(data=node_node_interactions, x="Passive Size", y="Active Size")
    plt.tight_layout()
    plt.savefig(os.path.splitext(filename)[0] + "_ratios.pdf")
    plt.clf()


def merge_benchmarks(pattern, output):
    benchmark_paths = glob.glob(pattern)
    tables = [pd.read_csv(p) for p in benchmark_paths]
    pd.concat(tables).to_csv(output, index=False)


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

    # print("Plotting exact fields")
    # zoom_box = plot_field('benchmarks/sample-exact-zoom.csv')
    # plot_field('benchmarks/sample-exact.csv', zoom_box)

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

    # print("Plotting tree fields")
    # plot_field('benchmarks/sample-vector-tree.csv')
    # plot_field('benchmarks/sample-quadrupole-tree.csv')
    # plot_field('benchmarks/sample-octupole-tree.csv')
    # plot_field('benchmarks/sample-hexadecupole-tree.csv')
    # plot_field('benchmarks/sample-triacontadyupole-tree.csv')

    # print("Plotting solver benchmarks")
    # plot_times_vs_n("benchmarks/all-solvers-random-data.csv")
    # plot_times_vs_n("remote-benchmarks/all-solvers-agora-data-constitutional.csv")
    # plot_interaction_counts_vs_n("benchmarks/all-solvers-random-data.csv", "mvdr")
    # plot_theta_vs_n("benchmarks/all-solvers-random-data.csv")
    # plot_interaction_counts_vs_n("benchmarks/all-solvers-random-data.csv")
    merge_benchmarks("remote-benchmarks/random/all-solvers-constitutional.*.csv",
                     "benchmarks/random/all-solvers-cluster-constitutional.csv")
    plot_times_vs_n("benchmarks/random/all-solvers-cluster-constitutional.csv", "Octupole")
    merge_benchmarks("remote-benchmarks/random/all-solvers-rms.*.csv",
                     "benchmarks/random/all-solvers-cluster-rms.csv")
    plot_times_vs_n("benchmarks/random/all-solvers-cluster-rms.csv", "Quadrupole")
    plot_times_vs_n("benchmarks/agora/all-solvers-constitutional.csv")

    solver_median_values_to_latex("benchmarks/agora/all-solvers-cluster-constitutional.csv")
    solver_median_values_to_latex("benchmarks/agora/all-solvers-cluster-rms.csv")

    # print("Plotting tree benchmarks")
    # plot_tree_sizes("benchmarks/tree-construction-agora-data.csv")
    # plot_tree_sizes("benchmarks/tree-construction-random-data.csv")
    # merge_benchmarks("remote-benchmarks/tree-construction-random-data.csv")
    # merge_benchmarks("remote-benchmarks/tree-construction-agora-data.csv")
    # plot_times_vs_n("remote-benchmarks/tree-construction-agora-data.csv")
    # plot_times_vs_n("remote-benchmarks/tree-construction-random-data.csv")

    # print("Plotting interaction counts")
    # plot_interaction_counts("benchmarks/interactions/all-solvers-count.csv")
    # plot_interaction_counts_vs_n("benchmarks/random/all-solvers.csv")

    # print("Plotting interaction lists")
    # plot_interaction_distances_vs_sizes("benchmarks/interactions/bh-2.csv", theta=0.44804686)
    # plot_interaction_distances_vs_sizes("benchmarks/interactions/lbvh-2.csv", theta=0.7082031)
    # plot_dual_interaction_distances_vs_sizes("benchmarks/interactions/mvdr-2.csv", theta=0.7082031 / 2)
    # plot_dual_interaction_distances_vs_sizes("benchmarks/interactions/fmm-2.csv", theta=0.41289064)
    # plot_dual_interaction_distances_vs_ratios("benchmarks/interactions/mvdr-2.csv")
    # plot_dual_interaction_distances_vs_ratios("benchmarks/interactions/fmm-2.csv")
    # plot_dual_interaction_sizes("benchmarks/interactions/mvdr-2.csv")
    # plot_dual_interaction_sizes("benchmarks/interactions/fmm-2.csv")

    print("Plotting theta comparisons")
    # plot_theta_vs_time_and_error("benchmarks/theta/bh.csv", "Quadrupole")
    # plot_error_vs_time("benchmarks/theta/bh.csv", "Quadrupole")
    plot_multipoles_vs_error_vs_time("benchmarks/theta/bh.csv")
    plot_multipoles_vs_error_vs_time("benchmarks/theta/lbvh.csv")
    plot_multipoles_vs_error_vs_time("benchmarks/theta/fmm.csv")
    plot_multipoles_vs_error_vs_time("benchmarks/theta/mvdr.csv")
    # plot_theta_vs_time_and_error("benchmarks/fmm-theta.csv", "Quadrupole")
    # plot_error_vs_time("benchmarks/fmm-theta.csv", "Quadrupole")


if __name__ == "__main__":
    main()
