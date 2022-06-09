# COVID-19

Code for the baseline scenario from the paper ["Modelling the impact of testing, contact tracing and household quarantine on second waves of COVID-19"](https://www.nature.com/articles/s41562-020-0931-9).

![Compartmental model](/model.png)

This code was also adapted and used in the paper ["Quantifying the importance and location of SARS-CoV-2 transmission events in large metropolitan areas"](https://doi.org/10.1073/pnas.2112182119).

## Sample input

There are three files in the Data folder:

- network.txt: network* edge list in the format *Source Target Weight*. The code assumes that the edge list is directed.
- nodes.txt: two-column file in the format *id age_group*. There are five age_groups: 0 [0-4], 1 [5-17], 2 [18-49], 3 [50-64], 4 [65,+)
- parameters_0.txt: file containing all the model parameters, see the paper for further details. Most parameters should be fixed, except for the first one (N, number of nodes) and the last one (beta, transmission probability). The transmission probability should be calibrated depending on the weights of the network under consideration to achieve the desired value of R0.

*Note: the original code was fed with mobility data obtained from a private company (see the paper for further details). The code in this repository is a simplified version that runs the baseline scenario on a random network with N=10.000 and \<k>=10.

## Networks from New York and Seattle Metropolitan Areas

The networks used in the paper "Quantifying the importance and location of SARS-CoV-2 transmission events in large metropolitan areas" have been deposited at zenodo: 

[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.6628727.svg)](https://doi.org/10.5281/zenodo.6628727)

They include the networks representing contacts in the community, workplace, household and school layers. The community and workplace networks were extracted from real mobility data (see the paper for further details).

## Usage

If the gnu scientific library is installed in the system, simply run
```
make
```
and then
```
./seir
```
This will produce 2 text files in the Results folder:

- cum_baseline_p0.txt: cumulative number of individuals in each state at the end of the simulation
- data_baseline_p0.txt: number of individuals in each state at each time step

## Citation

Please use either this citation:
```
Aleta, A., Martín-Corral, D., Pastore y Piontti, A. et al. Modelling the impact of testing, contact tracing and household quarantine on second waves of COVID-19. Nat Hum Behav 4, 964–971 (2020). https://doi.org/10.1038/s41562-020-0931-9
```

Corresponding to this bibtex entry:
```
@article{Aleta2020Sep,
	author = {Aleta, Alberto and Mart{\ifmmode\acute{\imath}\else\'{\i}\fi}n-Corral, David and Pastore y. Piontti, Ana and Ajelli, Marco and Litvinova, Maria and Chinazzi, Matteo and Dean, Natalie E. and Halloran, M. Elizabeth and Longini Ira, Jr., M. and Merler, Stefano and Pentland, Alex and Vespignani, Alessandro and Moro, Esteban and Moreno, Yamir},
	title = {{Modelling the impact of testing, contact tracing and household quarantine on second waves of COVID-19}},
	journal = {Nat. Hum. Behav.},
	volume = {4},
	number = {9},
	pages = {964--971},
	year = {2020},
	month = {Sep},
	issn = {2397-3374},
	publisher = {Nature Publishing Group},
	doi = {10.1038/s41562-020-0931-9}
}
```

Or this one:
```
Aleta, A. Martín-Corral, D., Bakker, M. A. et al. Quantifying the importance and location of SARS-CoV-2 transmission events in large metropolitan areas. PNAS (2022). https://doi.org/10.1073/pnas.2112182119
```

Corresponding to this bibtex entry:
```
@article{Aleta2022Jun,
        author = {Aleta, Alberto and Mart{\ifmmode\acute{\imath}\else\'{\i}\fi}n-Corral, Bakker, Michiel A. and David and Pastore y. Piontti, Ana and Ajelli, Marco and Litvinova, Maria and Chinazzi, Matteo and Dean, Natalie E. and Halloran, M. Elizabeth and Longini, Ira M. and Pentland, Alex and Vespignani, Alessandro and Moreno, Yamir and Moro, Esteban},
        title = {{Quantifying the importance and location of SARS-CoV-2 transmission events in large metropolitan areas}},
        journal = {PNAS},
        year = {2022},
        month = {Jun},
        doi = {10.1073/pnas.2112182119}
}
```

## License

[GPL](./LICENSE)
