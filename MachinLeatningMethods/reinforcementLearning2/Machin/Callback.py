from stable_baselines3.common.callbacks import BaseCallback

class ExplainedVarianceCallback(BaseCallback):
    def __init__(self, update_threshold, update_value, verbose=0):
        super(ExplainedVarianceCallback, self).__init__(verbose)
        self.update_threshold = update_threshold
        self.update_value = update_value

    def _on_step(self) -> bool:
        explained_variance = self.locals.get('explained_var', None)

        if explained_variance is not None and explained_variance > self.update_threshold:
            self.training_env.envs[0].length_coeff += self.update_value
            if self.verbose > 0:
                print(f"Updated length_coeff to {self.training_env.envs[0].length_coeff}")

        return True