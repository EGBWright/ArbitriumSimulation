require(ggplot2)
require(scales)

#These are attractive and color-blind friendly colors to use
colors <- c("#673F03", "#7D3002", "#891901", "#A7000F", "#B50142", "#CD0778", "#D506AD", "#E401E7", "#AB08FF","#7B1DFF", "#5731FD","#5E8EFF", "#4755FF" ,"#6FC4FE", "#86E9FE", "#96FFF7", "#B2FCE3", "#BBFFDB", "#D4FFDD", "#EFFDF0")

#Set your working directory
setwd("~/ArbitriumSimulation/Example/")

#You can have whatever variables you want like this
pop_cap <- 10000

#Read in your data
initial_data <- read.table("munged_basic.data", h=T)
initial_data
#You can add extra columns with calculations with cbin
initial_data <-cbind(initial_data, Period=as.factor(initial_data$period))

#You can get just a portion of your data with subset
early <- subset(initial_data, update<20)
just_middle <- subset(initial_data, period==25)

#If you have NaN values that you would like to be 0, this is how
zeroed <- initial_data
zeroed[is.na(zeroed)] <- 0

ggplot(data=initial_data, aes(x=update, y=mean_lysogeny_rate, group=Period, colour=Period)) + ylab("Mean Lysogeny Rate") + xlab("Updates") + stat_summary(aes(color=Period, fill=Period),fun.data="mean_cl_boot", geom=c("smooth"), se=TRUE) + theme(panel.background = element_rect(fill='white', colour='black')) + theme(panel.grid.major = element_blank(), panel.grid.minor = element_blank()) + guides(fill=FALSE) +scale_colour_manual(values=colors) + scale_fill_manual(values=colors) +xlim(0,100000)

#This will make a line plot over time
ggplot(data=initial_data, aes(x=update, y=mean_lysogeny_rate, group=period, colour=period)) + ylab("Mean Lysogeny Rate") + xlab("Updates") + stat_summary(aes(color=Period, fill= Period) ,fun.data="mean_cl_boot", geom=c("smooth"), se=TRUE) + theme(panel.background = element_rect(fill='white', colour='black')) + theme(panel.grid.major = element_blank(), panel.grid.minor = element_blank()) + guides(fill=FALSE) +scale_colour_manual(values=colors) + scale_fill_manual(values=colors) +xlim(0,100000) 

#update_1 <- subset(initial_data, update==1)

# This will make a point plot
ggplot(data=update_1, aes(x=count, y=coop, group=treatment, colour=treatment)) +ylab("Mean Cooperation Value") + xlab("Number of Organisms") + geom_point()

#Box plot
ggplot(data=update_1, aes(x=Treatment, y=coop)) + geom_boxplot()

# Non-parametric test to see if two sets of data are probably different
wilcox.test(subset(update_1, treatment==1.0)$coop, subset(update_1, treatment=0.5)$coop)