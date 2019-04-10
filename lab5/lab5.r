library(ggplot2)

# read data from CSV
results = read.csv("results.csv")

# aggregate data
avg = aggregate(cbind(naive, better, blas) ~ size, data=results, FUN=mean)

# standard deviation
avg$naive_sd = aggregate(naive ~ size, data=results, FUN=sd)$naive
avg$better_sd = aggregate(better ~ size, data=results, FUN=sd)$better
avg$blas_sd = aggregate(blas ~ size, data=results, FUN=sd)$blas

# naive polynomial fit
f_naive = lm(naive ~ poly(as.vector(avg[["size"]]), 3, raw=TRUE), data=avg)
p_naive = data.frame(x=avg[["size"]])
p_naive$y = predict(f_naive, p_naive)

# better polynomial fit
f_better = lm(better ~ poly(as.vector(avg[["size"]]), 3, raw=TRUE), data=avg)
p_better = data.frame(x=avg[["size"]])
p_better$y = predict(f_better, p_better)

# blas polynomial fit
f_blas = lm(blas ~ poly(as.vector(avg[["size"]]), 3, raw=TRUE), data=avg)
p_blas = data.frame(x=avg[["size"]])
p_blas$y = predict(f_blas, p_blas)

# plot the results
ggplot() +
  
  # polynomials
  geom_line(data=p_naive, aes(x, y), color="red") +
  geom_line(data=p_better, aes(x, y), color="blue") +
  geom_line(data=p_blas, aes(x, y), color="green") +
  
  # points
  geom_point(data=avg, aes(size, naive), color="red") +
  geom_point(data=avg, aes(size, better), color="blue") +
  geom_point(data=avg, aes(size, blas), color="green") +
  
  # error bars
  geom_errorbar(data=avg, aes(size, ymin=naive-naive_sd, ymax=naive+naive_sd)) +
  geom_errorbar(data=avg, aes(size, ymin=better-better_sd, ymax=better+better_sd)) +
  geom_errorbar(data=avg, aes(size, ymin=blas-blas_sd, ymax=blas+blas_sd))

  
